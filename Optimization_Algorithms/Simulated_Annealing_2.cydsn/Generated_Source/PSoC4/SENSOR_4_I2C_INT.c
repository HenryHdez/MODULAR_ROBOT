/***************************************************************************//**
* \file SENSOR_4_I2C_INT.c
* \version 3.20
*
* \brief
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in I2C mode.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SENSOR_4_PVT.h"
#include "SENSOR_4_I2C_PVT.h"
#include "cyapicallbacks.h"


/*******************************************************************************
* Function Name: SENSOR_4_I2C_ISR
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the SCB I2C mode.
*
*******************************************************************************/
CY_ISR(SENSOR_4_I2C_ISR)
{
    uint32 diffCount;
    uint32 endTransfer;

#ifdef SENSOR_4_I2C_ISR_ENTRY_CALLBACK
    SENSOR_4_I2C_ISR_EntryCallback();
#endif /* SENSOR_4_I2C_ISR_ENTRY_CALLBACK */

#if (SENSOR_4_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
    uint32 response;

    response = SENSOR_4_I2C_ACK_ADDR;
#endif /* (SENSOR_4_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */

    endTransfer = 0u; /* Continue active transfer */

    /* Calls customer routine if registered */
    if(NULL != SENSOR_4_customIntrHandler)
    {
        SENSOR_4_customIntrHandler();
    }

    if(SENSOR_4_CHECK_INTR_I2C_EC_MASKED(SENSOR_4_INTR_I2C_EC_WAKE_UP))
    {
        /* Mask-off after wakeup */
        SENSOR_4_SetI2CExtClkInterruptMode(SENSOR_4_NO_INTR_SOURCES);
    }

    /* Master and Slave error tracking:
    * Add the master state check to track only the master errors when the master is active or
    * track slave errors when the slave is active or idle.
    * A special MMS case: in the address phase with misplaced Start: the master sets the LOST_ARB and
    * slave BUS_ERR. The valid event is LOST_ARB comes from the master.
    */
    if(SENSOR_4_CHECK_I2C_FSM_MASTER)
    {
        #if(SENSOR_4_I2C_MASTER)
        {
            /* INTR_MASTER_I2C_BUS_ERROR:
            * A misplaced Start or Stop condition occurred on the bus: complete the transaction.
            * The interrupt is cleared in I2C_FSM_EXIT_IDLE.
            */
            if(SENSOR_4_CHECK_INTR_MASTER_MASKED(SENSOR_4_INTR_MASTER_I2C_BUS_ERROR))
            {
                SENSOR_4_mstrStatus |= (uint16) (SENSOR_4_I2C_MSTAT_ERR_XFER |
                                                         SENSOR_4_I2C_MSTAT_ERR_BUS_ERROR);

                endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER;
            }

            /* INTR_MASTER_I2C_ARB_LOST:
            * The MultiMaster lost arbitrage during transaction.
            * A Misplaced Start or Stop condition is treated as lost arbitration when the master drives the SDA.
            * The interrupt source is cleared in I2C_FSM_EXIT_IDLE.
            */
            if(SENSOR_4_CHECK_INTR_MASTER_MASKED(SENSOR_4_INTR_MASTER_I2C_ARB_LOST))
            {
                SENSOR_4_mstrStatus |= (uint16) (SENSOR_4_I2C_MSTAT_ERR_XFER |
                                                         SENSOR_4_I2C_MSTAT_ERR_ARB_LOST);

                endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER;
            }

            #if(SENSOR_4_I2C_MULTI_MASTER_SLAVE)
            {
                /* I2C_MASTER_CMD_M_START_ON_IDLE:
                * MultiMaster-Slave does not generate start, because Slave was addressed.
                * Pass control to slave.
                */
                if(SENSOR_4_CHECK_I2C_MASTER_CMD(SENSOR_4_I2C_MASTER_CMD_M_START_ON_IDLE))
                {
                    SENSOR_4_mstrStatus |= (uint16) (SENSOR_4_I2C_MSTAT_ERR_XFER |
                                                             SENSOR_4_I2C_MSTAT_ERR_ABORT_XFER);

                    endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER;
                }
            }
            #endif

            /* The error handling common part:
            * Sets a completion flag of the master transaction and passes control to:
            *  - I2C_FSM_EXIT_IDLE - to complete transaction in case of: ARB_LOST or BUS_ERR.
            *  - I2C_FSM_IDLE      - to take chance for the slave to process incoming transaction.
            */
            if(0u != endTransfer)
            {
                /* Set completion flags for master */
                SENSOR_4_mstrStatus |= (uint16) SENSOR_4_GET_I2C_MSTAT_CMPLT;

                #if(SENSOR_4_I2C_MULTI_MASTER_SLAVE)
                {
                    if(SENSOR_4_CHECK_I2C_FSM_ADDR)
                    {
                        /* Start generation is set after another master starts accessing Slave.
                        * Clean-up master and turn to slave. Set state to IDLE.
                        */
                        if(SENSOR_4_CHECK_I2C_MASTER_CMD(SENSOR_4_I2C_MASTER_CMD_M_START_ON_IDLE))
                        {
                            SENSOR_4_I2C_MASTER_CLEAR_START;

                            endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER; /* Pass control to Slave */
                        }
                        /* Valid arbitration lost on the address phase happens only when: master LOST_ARB is set and
                        * slave BUS_ERR is cleared. Only in that case set the state to IDLE without SCB IP re-enable.
                        */
                        else if((!SENSOR_4_CHECK_INTR_SLAVE_MASKED(SENSOR_4_INTR_SLAVE_I2C_BUS_ERROR))
                               && SENSOR_4_CHECK_INTR_MASTER_MASKED(SENSOR_4_INTR_MASTER_I2C_ARB_LOST))
                        {
                            endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER; /* Pass control to Slave */
                        }
                        else
                        {
                            endTransfer = 0u; /* Causes I2C_FSM_EXIT_IDLE to be set below */
                        }

                        if(0u != endTransfer) /* Clean-up master to proceed with slave */
                        {
                            SENSOR_4_CLEAR_TX_FIFO; /* Shifter keeps address, clear it */

                            SENSOR_4_DISABLE_MASTER_AUTO_DATA_ACK; /* In case of reading disable autoACK */

                            /* Clean-up master interrupt sources */
                            SENSOR_4_ClearMasterInterruptSource(SENSOR_4_INTR_MASTER_ALL);

                            /* Disable data processing interrupts: they have to be cleared before */
                            SENSOR_4_SetRxInterruptMode(SENSOR_4_NO_INTR_SOURCES);
                            SENSOR_4_SetTxInterruptMode(SENSOR_4_NO_INTR_SOURCES);

                            SENSOR_4_state = SENSOR_4_I2C_FSM_IDLE;
                        }
                        else
                        {
                            /* Set I2C_FSM_EXIT_IDLE for BUS_ERR and ARB_LOST (that is really bus error) */
                            SENSOR_4_state = SENSOR_4_I2C_FSM_EXIT_IDLE;
                        }
                    }
                    else
                    {
                        /* Set I2C_FSM_EXIT_IDLE if any other state than address */
                        SENSOR_4_state = SENSOR_4_I2C_FSM_EXIT_IDLE;
                    }
                }
                #else
                {
                    /* In case of LOST*/
                    SENSOR_4_state = SENSOR_4_I2C_FSM_EXIT_IDLE;
                }
                #endif
            }
        }
        #endif
    }
    else /* (SENSOR_4_CHECK_I2C_FSM_SLAVE) */
    {
        #if(SENSOR_4_I2C_SLAVE)
        {
            /* INTR_SLAVE_I2C_BUS_ERROR or SENSOR_4_INTR_SLAVE_I2C_ARB_LOST:
            * A Misplaced Start or Stop condition occurred on the bus: set a flag
            * to notify an error condition.
            */
            if(SENSOR_4_CHECK_INTR_SLAVE_MASKED(SENSOR_4_INTR_SLAVE_I2C_BUS_ERROR |
                                                        SENSOR_4_INTR_SLAVE_I2C_ARB_LOST))
            {
                if(SENSOR_4_CHECK_I2C_FSM_RD)
                {
                    /* TX direction: master reads from slave */
                    SENSOR_4_slStatus &= (uint8) ~SENSOR_4_I2C_SSTAT_RD_BUSY;
                    SENSOR_4_slStatus |= (uint8) (SENSOR_4_I2C_SSTAT_RD_ERR |
                                                          SENSOR_4_I2C_SSTAT_RD_CMPLT);
                }
                else
                {
                    /* RX direction: master writes into slave */
                    SENSOR_4_slStatus &= (uint8) ~SENSOR_4_I2C_SSTAT_WR_BUSY;
                    SENSOR_4_slStatus |= (uint8) (SENSOR_4_I2C_SSTAT_WR_ERR |
                                                          SENSOR_4_I2C_SSTAT_WR_CMPLT);
                }

                SENSOR_4_state = SENSOR_4_I2C_FSM_EXIT_IDLE;
            }
        }
        #endif
    }

    /* States description:
    * Any Master operation starts from: the ADDR_RD/WR state as the master generates traffic on the bus.
    * Any Slave operation starts from: the IDLE state as the slave always waits for actions from the master.
    */

    /* FSM Master */
    if(SENSOR_4_CHECK_I2C_FSM_MASTER)
    {
        #if(SENSOR_4_I2C_MASTER)
        {
            /* INTR_MASTER_I2C_STOP:
            * A Stop condition was generated by the master: the end of the transaction.
            * Set completion flags to notify the API.
            */
            if(SENSOR_4_CHECK_INTR_MASTER_MASKED(SENSOR_4_INTR_MASTER_I2C_STOP))
            {
                SENSOR_4_ClearMasterInterruptSource(SENSOR_4_INTR_MASTER_I2C_STOP);

                SENSOR_4_mstrStatus |= (uint16) SENSOR_4_GET_I2C_MSTAT_CMPLT;
                SENSOR_4_state       = SENSOR_4_I2C_FSM_IDLE;
            }
            else
            {
                if(SENSOR_4_CHECK_I2C_FSM_ADDR) /* Address stage */
                {
                    /* INTR_MASTER_I2C_NACK:
                    * The master sent an address but it was NACKed by the slave. Complete transaction.
                    */
                    if(SENSOR_4_CHECK_INTR_MASTER_MASKED(SENSOR_4_INTR_MASTER_I2C_NACK))
                    {
                        SENSOR_4_ClearMasterInterruptSource(SENSOR_4_INTR_MASTER_I2C_NACK);

                        SENSOR_4_mstrStatus |= (uint16) (SENSOR_4_I2C_MSTAT_ERR_XFER |
                                                                 SENSOR_4_I2C_MSTAT_ERR_ADDR_NAK);

                        endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER;
                    }
                    /* INTR_TX_UNDERFLOW. The master sent an address:
                    *  - TX direction: the clock is stretched after the ACK phase, because the TX FIFO is
                    *    EMPTY. The TX EMPTY cleans all the TX interrupt sources.
                    *  - RX direction: the 1st byte is received, but there is no ACK permission,
                    *    the clock is stretched after 1 byte is received.
                    */
                    else
                    {
                        if(SENSOR_4_CHECK_I2C_FSM_RD) /* Reading */
                        {
                            SENSOR_4_state = SENSOR_4_I2C_FSM_MSTR_RD_DATA;
                        }
                        else /* Writing */
                        {
                            SENSOR_4_state = SENSOR_4_I2C_FSM_MSTR_WR_DATA;
                            if(0u != SENSOR_4_mstrWrBufSize)
                            {
                                /* Enable INTR.TX_EMPTY if there is data to transmit */
                                SENSOR_4_SetTxInterruptMode(SENSOR_4_INTR_TX_EMPTY);
                            }
                        }
                    }
                }

                if(SENSOR_4_CHECK_I2C_FSM_DATA) /* Data phase */
                {
                    if(SENSOR_4_CHECK_I2C_FSM_RD) /* Reading */
                    {
                        /* INTR_RX_FULL:
                        * RX direction: the master received 8 bytes.
                        * Get data from RX FIFO and decide whether to ACK or  NACK the following bytes.
                        */
                        if(SENSOR_4_CHECK_INTR_RX_MASKED(SENSOR_4_INTR_RX_FULL))
                        {
                            /* Calculate difference */
                            diffCount =  SENSOR_4_mstrRdBufSize -
                                        (SENSOR_4_mstrRdBufIndex + SENSOR_4_GET_RX_FIFO_ENTRIES);

                            /* Proceed transaction or end it when RX FIFO becomes FULL again */
                            if(diffCount > SENSOR_4_I2C_FIFO_SIZE)
                            {
                                diffCount = SENSOR_4_I2C_FIFO_SIZE;
                            }
                            else
                            {
                                if(0u == diffCount)
                                {
                                    SENSOR_4_DISABLE_MASTER_AUTO_DATA_ACK;

                                    diffCount   = SENSOR_4_I2C_FIFO_SIZE;
                                    endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER;
                                }
                            }

                            for(; (0u != diffCount); diffCount--)
                            {
                                SENSOR_4_mstrRdBufPtr[SENSOR_4_mstrRdBufIndex] = (uint8)
                                                                                        SENSOR_4_RX_FIFO_RD_REG;
                                SENSOR_4_mstrRdBufIndex++;
                            }
                        }
                        /* INTR_RX_NOT_EMPTY:
                        * RX direction: the master received one data byte, ACK or NACK it.
                        * The last byte is stored and NACKed by the master. The NACK and Stop is
                        * generated by one command generate Stop.
                        */
                        else if(SENSOR_4_CHECK_INTR_RX_MASKED(SENSOR_4_INTR_RX_NOT_EMPTY))
                        {
                            /* Put data in component buffer */
                            SENSOR_4_mstrRdBufPtr[SENSOR_4_mstrRdBufIndex] = (uint8) SENSOR_4_RX_FIFO_RD_REG;
                            SENSOR_4_mstrRdBufIndex++;

                            if(SENSOR_4_mstrRdBufIndex < SENSOR_4_mstrRdBufSize)
                            {
                                SENSOR_4_I2C_MASTER_GENERATE_ACK;
                            }
                            else
                            {
                               endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER;
                            }
                        }
                        else
                        {
                            /* Do nothing */
                        }

                        SENSOR_4_ClearRxInterruptSource(SENSOR_4_INTR_RX_ALL);
                    }
                    else /* Writing */
                    {
                        /* INTR_MASTER_I2C_NACK :
                        * The master writes data to the slave and NACK was received: not all the bytes were
                        * written to the slave from the TX FIFO. Revert the index if there is data in
                        * the TX FIFO and pass control to a complete transfer.
                        */
                        if(SENSOR_4_CHECK_INTR_MASTER_MASKED(SENSOR_4_INTR_MASTER_I2C_NACK))
                        {
                            SENSOR_4_ClearMasterInterruptSource(SENSOR_4_INTR_MASTER_I2C_NACK);

                            /* Rollback write buffer index: NACKed byte remains in shifter */
                            SENSOR_4_mstrWrBufIndexTmp -= (SENSOR_4_GET_TX_FIFO_ENTRIES +
                                                                   SENSOR_4_GET_TX_FIFO_SR_VALID);

                            /* Update number of transferred bytes */
                            SENSOR_4_mstrWrBufIndex = SENSOR_4_mstrWrBufIndexTmp;

                            SENSOR_4_mstrStatus |= (uint16) (SENSOR_4_I2C_MSTAT_ERR_XFER |
                                                                     SENSOR_4_I2C_MSTAT_ERR_SHORT_XFER);

                            SENSOR_4_CLEAR_TX_FIFO;

                            endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER;
                        }
                        /* INTR_TX_EMPTY :
                        * TX direction: the TX FIFO is EMPTY, the data from the buffer needs to be put there.
                        * When there is no data in the component buffer, the underflow interrupt is
                        * enabled to catch when all the data has been transferred.
                        */
                        else if(SENSOR_4_CHECK_INTR_TX_MASKED(SENSOR_4_INTR_TX_EMPTY))
                        {
                            while(SENSOR_4_I2C_FIFO_SIZE != SENSOR_4_GET_TX_FIFO_ENTRIES)
                            {
                                /* The temporary mstrWrBufIndexTmp is used because slave could NACK the byte and index
                                * roll-back required in this case. The mstrWrBufIndex is updated at the end of transfer.
                                */
                                if(SENSOR_4_mstrWrBufIndexTmp < SENSOR_4_mstrWrBufSize)
                                {
                                #if(!SENSOR_4_CY_SCBIP_V0)
                                   /* Clear INTR_TX.UNDERFLOW before putting the last byte into TX FIFO. This ensures
                                    * a proper trigger at the end of transaction when INTR_TX.UNDERFLOW single trigger
                                    * event. Ticket ID# 156735.
                                    */
                                    if(SENSOR_4_mstrWrBufIndexTmp == (SENSOR_4_mstrWrBufSize - 1u))
                                    {
                                        SENSOR_4_ClearTxInterruptSource(SENSOR_4_INTR_TX_UNDERFLOW);
                                        SENSOR_4_SetTxInterruptMode(SENSOR_4_INTR_TX_UNDERFLOW);
                                    }
                                 #endif /* (!SENSOR_4_CY_SCBIP_V0) */

                                    /* Put data into TX FIFO */
                                    SENSOR_4_TX_FIFO_WR_REG = (uint32) SENSOR_4_mstrWrBufPtr[SENSOR_4_mstrWrBufIndexTmp];
                                    SENSOR_4_mstrWrBufIndexTmp++;
                                }
                                else
                                {
                                    break; /* No more data to put */
                                }
                            }

                        #if(SENSOR_4_CY_SCBIP_V0)
                            if(SENSOR_4_mstrWrBufIndexTmp == SENSOR_4_mstrWrBufSize)
                            {
                                SENSOR_4_SetTxInterruptMode(SENSOR_4_INTR_TX_UNDERFLOW);
                            }

                            SENSOR_4_ClearTxInterruptSource(SENSOR_4_INTR_TX_ALL);
                        #else
                            SENSOR_4_ClearTxInterruptSource(SENSOR_4_INTR_TX_EMPTY);
                        #endif /* (SENSOR_4_CY_SCBIP_V0) */
                        }
                        /* INTR_TX_UNDERFLOW:
                        * TX direction: all data from the TX FIFO was transferred to the slave.
                        * The transaction needs to be completed.
                        */
                        else if(SENSOR_4_CHECK_INTR_TX_MASKED(SENSOR_4_INTR_TX_UNDERFLOW))
                        {
                            /* Update number of transferred bytes */
                            SENSOR_4_mstrWrBufIndex = SENSOR_4_mstrWrBufIndexTmp;

                            endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER;
                        }
                        else
                        {
                            /* Do nothing */
                        }
                    }
                }

                if(0u != endTransfer) /* Complete transfer */
                {
                    /* Clean-up master after reading: only in case of NACK */
                    SENSOR_4_DISABLE_MASTER_AUTO_DATA_ACK;

                    /* Disable data processing interrupts: they have to be cleared before */
                    SENSOR_4_SetRxInterruptMode(SENSOR_4_NO_INTR_SOURCES);
                    SENSOR_4_SetTxInterruptMode(SENSOR_4_NO_INTR_SOURCES);

                    if(SENSOR_4_CHECK_I2C_MODE_NO_STOP(SENSOR_4_mstrControl))
                    {
                        /* On-going transaction is suspended: the ReStart is generated by the API request */
                        SENSOR_4_mstrStatus |= (uint16) (SENSOR_4_I2C_MSTAT_XFER_HALT |
                                                                 SENSOR_4_GET_I2C_MSTAT_CMPLT);

                        SENSOR_4_state = SENSOR_4_I2C_FSM_MSTR_HALT;
                    }
                    else
                    {
                        /* Complete transaction: exclude the data processing state and generate Stop.
                        * The completion status will be set after Stop generation.
                        * A special case is read: because NACK and Stop are generated by the command below.
                        * Lost arbitration can occur during NACK generation when
                        * the other master is still reading from the slave.
                        */
                        SENSOR_4_I2C_MASTER_GENERATE_STOP;
                    }
                }
            }

        } /* (SENSOR_4_I2C_MASTER) */
        #endif

    } /* (SENSOR_4_CHECK_I2C_FSM_MASTER) */


    /* FSM Slave */
    else if(SENSOR_4_CHECK_I2C_FSM_SLAVE)
    {
        #if(SENSOR_4_I2C_SLAVE)
        {
            /* INTR_SLAVE_NACK:
            * The master completes reading the slave: the appropriate flags have to be set.
            * The TX FIFO is cleared after an overflow condition is set.
            */
            if(SENSOR_4_CHECK_INTR_SLAVE_MASKED(SENSOR_4_INTR_SLAVE_I2C_NACK))
            {
                SENSOR_4_ClearSlaveInterruptSource(SENSOR_4_INTR_SLAVE_I2C_NACK);

                /* All entries that remain in TX FIFO are: FIFO Size + 1 (SHIFTER) */
                diffCount = (SENSOR_4_GET_TX_FIFO_ENTRIES + SENSOR_4_GET_TX_FIFO_SR_VALID);

                if(SENSOR_4_slOverFlowCount > diffCount) /* Overflow */
                {
                    SENSOR_4_slStatus |= (uint8) SENSOR_4_I2C_SSTAT_RD_OVFL;
                }
                else /* No Overflow */
                {
                    /* Roll-back temporary index */
                    SENSOR_4_slRdBufIndexTmp -= (diffCount - SENSOR_4_slOverFlowCount);
                }

                /* Update slave of transferred bytes */
                SENSOR_4_slRdBufIndex = SENSOR_4_slRdBufIndexTmp;

                /* Clean-up TX FIFO */
                SENSOR_4_SetTxInterruptMode(SENSOR_4_NO_INTR_SOURCES);
                SENSOR_4_slOverFlowCount = 0u;
                SENSOR_4_CLEAR_TX_FIFO;

                /* Complete master reading */
                SENSOR_4_slStatus &= (uint8) ~SENSOR_4_I2C_SSTAT_RD_BUSY;
                SENSOR_4_slStatus |= (uint8)  SENSOR_4_I2C_SSTAT_RD_CMPLT;
                SENSOR_4_state     =  SENSOR_4_I2C_FSM_IDLE;
            }


            /* INTR_SLAVE_I2C_WRITE_STOP:
            * The master completes writing to the slave: the appropriate flags have to be set.
            * The RX FIFO contains 1-8 bytes from the previous transaction which needs to be read.
            * There is a possibility that RX FIFO contains an address, it needs to leave it there.
            */
            if(SENSOR_4_CHECK_INTR_SLAVE_MASKED(SENSOR_4_INTR_SLAVE_I2C_WRITE_STOP))
            {
                SENSOR_4_ClearSlaveInterruptSource(SENSOR_4_INTR_SLAVE_I2C_WRITE_STOP);

                /* Read bytes from RX FIFO when auto data ACK receive logic is enabled. Otherwise all data bytes
                * were already read from the RX FIFO except for address byte which has to stay here to be handled by
                * I2C_ADDR_MATCH.
                */
                if (0u != (SENSOR_4_I2C_CTRL_REG & SENSOR_4_I2C_CTRL_S_READY_DATA_ACK))
                {
                    while(0u != SENSOR_4_GET_RX_FIFO_ENTRIES)
                    {
                        #if(SENSOR_4_CHECK_I2C_ACCEPT_ADDRESS)
                        {
                            if((1u == SENSOR_4_GET_RX_FIFO_ENTRIES) &&
                               (SENSOR_4_CHECK_INTR_SLAVE_MASKED(SENSOR_4_INTR_SLAVE_I2C_ADDR_MATCH)))
                            {
                                break; /* Leave address in RX FIFO */
                            }
                        }
                        #endif

                        /* Put data in component buffer */
                        SENSOR_4_slWrBufPtr[SENSOR_4_slWrBufIndex] = (uint8) SENSOR_4_RX_FIFO_RD_REG;
                        SENSOR_4_slWrBufIndex++;
                    }

                    SENSOR_4_DISABLE_SLAVE_AUTO_DATA;
                }

                if(SENSOR_4_CHECK_INTR_RX(SENSOR_4_INTR_RX_OVERFLOW))
                {
                    SENSOR_4_slStatus |= (uint8) SENSOR_4_I2C_SSTAT_WR_OVFL;
                }

                /* Clears RX interrupt sources triggered on data receiving */
                SENSOR_4_SetRxInterruptMode(SENSOR_4_NO_INTR_SOURCES);
                SENSOR_4_ClearRxInterruptSource(SENSOR_4_INTR_RX_ALL);

                /* Complete master writing */
                SENSOR_4_slStatus &= (uint8) ~SENSOR_4_I2C_SSTAT_WR_BUSY;
                SENSOR_4_slStatus |= (uint8)  SENSOR_4_I2C_SSTAT_WR_CMPLT;
                SENSOR_4_state     =  SENSOR_4_I2C_FSM_IDLE;
            }


            /* INTR_SLAVE_I2C_ADDR_MATCH or INTR_SLAVE_I2C_GENERAL:
            * The address match or general call address event starts the slave operation:
            * after leaving the TX or RX direction has to be chosen.
            * The wakeup interrupt must be cleared only after an address match is set.
            */
        #if (SENSOR_4_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
            if (SENSOR_4_CHECK_INTR_SLAVE_MASKED(SENSOR_4_INTR_SLAVE_I2C_ADDR_MATCH |
                                                         SENSOR_4_INTR_SLAVE_I2C_GENERAL))
        #else
            if (SENSOR_4_CHECK_INTR_SLAVE_MASKED(SENSOR_4_INTR_SLAVE_I2C_ADDR_MATCH))
        #endif /* (SENSOR_4_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */
            {
                /* Clear externally clocked address match interrupt source when internally clocked is set */
                SENSOR_4_ClearI2CExtClkInterruptSource(SENSOR_4_INTR_I2C_EC_WAKE_UP);

                #if (SENSOR_4_I2C_CUSTOM_ADDRESS_HANDLER)
                {
                    if (NULL != SENSOR_4_customAddressHandler)
                    {
                        /* Call custom address handler */
                        response = SENSOR_4_customAddressHandler();
                    }
                    else
                    {
                        /* Read address from the RX FIFO. If there is no address underflow triggers but
                        * component does not use that source. */
                        (void) SENSOR_4_RX_FIFO_RD_REG;
                        response = SENSOR_4_I2C_ACK_ADDR;
                    }

                    /* Clears RX sources after address was received in the RX FIFO */
                    SENSOR_4_ClearRxInterruptSource(SENSOR_4_INTR_RX_ALL);
                }
                #endif

            #if (SENSOR_4_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
                if (response == SENSOR_4_I2C_NAK_ADDR)
                {
                #if (!SENSOR_4_CY_SCBIP_V0)
                    /* Disable write stop interrupt source as it triggers after address was NACKed. Ticket ID#156094 */
                    SENSOR_4_DISABLE_INTR_SLAVE(SENSOR_4_INTR_SLAVE_I2C_WRITE_STOP);
                #endif /* (!SENSOR_4_CY_SCBIP_V0) */

                    /* Clear address match and stop history */
                    SENSOR_4_ClearSlaveInterruptSource(SENSOR_4_INTR_SLAVE_ALL);

                    /* ACK the address byte */
                    SENSOR_4_I2C_SLAVE_GENERATE_NACK;
                }
                else
            #endif /* (SENSOR_4_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */
                {
                    if(SENSOR_4_CHECK_I2C_STATUS(SENSOR_4_I2C_STATUS_S_READ))
                    /* TX direction: master reads from slave */
                    {
                        SENSOR_4_SetTxInterruptMode(SENSOR_4_INTR_TX_EMPTY);

                        /* Set temporary index to address buffer clear from API */
                        SENSOR_4_slRdBufIndexTmp = SENSOR_4_slRdBufIndex;

                        /* Start master reading */
                        SENSOR_4_slStatus |= (uint8) SENSOR_4_I2C_SSTAT_RD_BUSY;
                        SENSOR_4_state     = SENSOR_4_I2C_FSM_SL_RD;
                    }
                    else
                    /* RX direction: master writes into slave */
                    {
                        /* Calculate available buffer size */
                        diffCount = (SENSOR_4_slWrBufSize - SENSOR_4_slWrBufIndex);

                    #if (SENSOR_4_CY_SCBIP_V0)
                        if(diffCount < SENSOR_4_I2C_FIFO_SIZE)
                        /* Receive data: byte-by-byte */
                        {
                            SENSOR_4_SetRxInterruptMode(SENSOR_4_INTR_RX_NOT_EMPTY);
                        }
                        else
                        /* Receive data: into RX FIFO */
                        {
                            if(diffCount == SENSOR_4_I2C_FIFO_SIZE)
                            {
                                /* NACK when RX FIFO become FULL */
                                SENSOR_4_ENABLE_SLAVE_AUTO_DATA;
                            }
                            else
                            {
                                /* Stretch clock when RX FIFO becomes FULL */
                                SENSOR_4_ENABLE_SLAVE_AUTO_DATA_ACK;
                                SENSOR_4_SetRxInterruptMode(SENSOR_4_INTR_RX_FULL);
                            }
                        }

                    #else
                        #if(SENSOR_4_CHECK_I2C_ACCEPT_ADDRESS)
                        {
                            /* Enable RX.NOT_EMPTY interrupt source to receive byte by byte.
                            * The byte by byte receive is always chosen for the case when an address is accepted
                            * in RX FIFO. Ticket ID#175559.
                            */
                            SENSOR_4_SetRxInterruptMode(SENSOR_4_INTR_RX_NOT_EMPTY);
                        }
                        #else
                        {
                            if(diffCount < SENSOR_4_I2C_FIFO_SIZE)
                            /* Receive data: byte-by-byte */
                            {
                                SENSOR_4_SetRxInterruptMode(SENSOR_4_INTR_RX_NOT_EMPTY);
                            }
                            else
                            /* Receive data: into RX FIFO */
                            {
                                if(diffCount == SENSOR_4_I2C_FIFO_SIZE)
                                {
                                    /* NACK when RX FIFO become FULL */
                                    SENSOR_4_ENABLE_SLAVE_AUTO_DATA;
                                }
                                else
                                {
                                    /* Stretch clock when RX FIFO becomes FULL */
                                    SENSOR_4_ENABLE_SLAVE_AUTO_DATA_ACK;
                                    SENSOR_4_SetRxInterruptMode(SENSOR_4_INTR_RX_FULL);
                                }
                            }
                        }
                        #endif
                    #endif /* (SENSOR_4_CY_SCBIP_V0) */

                        /* Start master reading */
                        SENSOR_4_slStatus |= (uint8) SENSOR_4_I2C_SSTAT_WR_BUSY;
                        SENSOR_4_state     = SENSOR_4_I2C_FSM_SL_WR;
                    }

                    /* Clear address match and stop history */
                    SENSOR_4_ClearSlaveInterruptSource(SENSOR_4_INTR_SLAVE_ALL);

                #if (!SENSOR_4_CY_SCBIP_V0)
                    /* Enable write stop interrupt source as it triggers after address was NACKed. Ticket ID#156094 */
                    SENSOR_4_ENABLE_INTR_SLAVE(SENSOR_4_INTR_SLAVE_I2C_WRITE_STOP);
                #endif /* (!SENSOR_4_CY_SCBIP_V0) */

                    /* ACK the address byte */
                    SENSOR_4_I2C_SLAVE_GENERATE_ACK;
                }
            }

            /* SENSOR_4_INTR_RX_FULL:
            * Get data from the RX FIFO and decide whether to ACK or NACK the following bytes
            */
            if(SENSOR_4_CHECK_INTR_RX_MASKED(SENSOR_4_INTR_RX_FULL))
            {
                /* Calculate available buffer size to take into account that RX FIFO is FULL */
                diffCount =  SENSOR_4_slWrBufSize -
                            (SENSOR_4_slWrBufIndex + SENSOR_4_I2C_FIFO_SIZE);

                if(diffCount > SENSOR_4_I2C_FIFO_SIZE) /* Proceed transaction */
                {
                    diffCount   = SENSOR_4_I2C_FIFO_SIZE;
                    endTransfer = 0u;  /* Continue active transfer */
                }
                else /* End when FIFO becomes FULL again */
                {
                    endTransfer = SENSOR_4_I2C_CMPLT_ANY_TRANSFER;
                }

                for(; (0u != diffCount); diffCount--)
                {
                    /* Put data in component buffer */
                    SENSOR_4_slWrBufPtr[SENSOR_4_slWrBufIndex] = (uint8) SENSOR_4_RX_FIFO_RD_REG;
                    SENSOR_4_slWrBufIndex++;
                }

                if(0u != endTransfer) /* End transfer sending NACK */
                {
                    SENSOR_4_ENABLE_SLAVE_AUTO_DATA_NACK;

                    /* INTR_RX_FULL triggers earlier than INTR_SLAVE_I2C_STOP:
                    * disable all RX interrupt sources.
                    */
                    SENSOR_4_SetRxInterruptMode(SENSOR_4_NO_INTR_SOURCES);
                }

                SENSOR_4_ClearRxInterruptSource(SENSOR_4_INTR_RX_FULL);
            }
            /* SENSOR_4_INTR_RX_NOT_EMPTY:
            * The buffer size is less than 8: it requires processing in byte-by-byte mode.
            */
            else if(SENSOR_4_CHECK_INTR_RX_MASKED(SENSOR_4_INTR_RX_NOT_EMPTY))
            {
                diffCount = SENSOR_4_RX_FIFO_RD_REG;

                if(SENSOR_4_slWrBufIndex < SENSOR_4_slWrBufSize)
                {
                    SENSOR_4_I2C_SLAVE_GENERATE_ACK;

                    /* Put data into component buffer */
                    SENSOR_4_slWrBufPtr[SENSOR_4_slWrBufIndex] = (uint8) diffCount;
                    SENSOR_4_slWrBufIndex++;
                }
                else /* Overflow: there is no space in write buffer */
                {
                    SENSOR_4_I2C_SLAVE_GENERATE_NACK;

                    SENSOR_4_slStatus |= (uint8) SENSOR_4_I2C_SSTAT_WR_OVFL;
                }

                SENSOR_4_ClearRxInterruptSource(SENSOR_4_INTR_RX_NOT_EMPTY);
            }
            else
            {
                /* Does nothing */
            }


            /* SENSOR_4_INTR_TX_EMPTY:
            * The master reads the slave: provide data to read or 0xFF in the case of the end of the buffer
            * The overflow condition must be captured, but not set until the end of transaction.
            * There is a possibility of a false overflow due to TX FIFO utilization.
            */
            if(SENSOR_4_CHECK_INTR_TX_MASKED(SENSOR_4_INTR_TX_EMPTY))
            {
                while(SENSOR_4_I2C_FIFO_SIZE != SENSOR_4_GET_TX_FIFO_ENTRIES)
                {
                    /* Temporary slRdBufIndexTmp is used because the master can NACK the byte and
                    * index roll-back is required in this case. The slRdBufIndex is updated at the end
                    * of the read transfer.
                    */
                    if(SENSOR_4_slRdBufIndexTmp < SENSOR_4_slRdBufSize)
                    /* Data from buffer */
                    {
                        SENSOR_4_TX_FIFO_WR_REG = (uint32) SENSOR_4_slRdBufPtr[SENSOR_4_slRdBufIndexTmp];
                        SENSOR_4_slRdBufIndexTmp++;
                    }
                    else
                    /* Probably Overflow */
                    {
                        SENSOR_4_TX_FIFO_WR_REG = SENSOR_4_I2C_SLAVE_OVFL_RETURN;

                        if(SENSOR_4_slOverFlowCount <= SENSOR_4_I2C_TX_OVERFLOW_COUNT)
                        {
                            /* Get counter in range of overflow. */
                            SENSOR_4_slOverFlowCount++;
                        }
                    }
                }

                SENSOR_4_ClearTxInterruptSource(SENSOR_4_INTR_TX_EMPTY);
            }

        }  /* (SENSOR_4_I2C_SLAVE) */
        #endif
    }


    /* FSM EXIT:
    * Slave:  INTR_SLAVE_I2C_BUS_ERROR, INTR_SLAVE_I2C_ARB_LOST
    * Master: INTR_MASTER_I2C_BUS_ERROR, INTR_MASTER_I2C_ARB_LOST.
    */
    else
    {
        SENSOR_4_CTRL_REG &= (uint32) ~SENSOR_4_CTRL_ENABLED; /* Disable scb IP */

        SENSOR_4_state = SENSOR_4_I2C_FSM_IDLE;

        SENSOR_4_DISABLE_SLAVE_AUTO_DATA;
        SENSOR_4_DISABLE_MASTER_AUTO_DATA;

    #if(SENSOR_4_CY_SCBIP_V0)
        SENSOR_4_SetRxInterruptMode(SENSOR_4_NO_INTR_SOURCES);
        SENSOR_4_SetTxInterruptMode(SENSOR_4_NO_INTR_SOURCES);

        /* Clear interrupt sources as they are not automatically cleared after SCB is disabled */
        SENSOR_4_ClearTxInterruptSource(SENSOR_4_INTR_RX_ALL);
        SENSOR_4_ClearRxInterruptSource(SENSOR_4_INTR_TX_ALL);
        SENSOR_4_ClearSlaveInterruptSource(SENSOR_4_INTR_SLAVE_ALL);
        SENSOR_4_ClearMasterInterruptSource(SENSOR_4_INTR_MASTER_ALL);
    #endif /* (SENSOR_4_CY_SCBIP_V0) */

        SENSOR_4_CTRL_REG |= (uint32) SENSOR_4_CTRL_ENABLED;  /* Enable scb IP */
    }

#ifdef SENSOR_4_I2C_ISR_EXIT_CALLBACK
    SENSOR_4_I2C_ISR_ExitCallback();
#endif /* SENSOR_4_I2C_ISR_EXIT_CALLBACK */

}


/* [] END OF FILE */
