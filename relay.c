/*
 * This file is part of the firmware for yogurt maker project
 * (https://github.com/mister-grumbler/yogurt-maker).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Control functions for relay.
 */

#include "relay.h"
#include "stm8s003/gpio.h"
#include "adc.h"
#include "timer.h"
#include "params.h"

#define RELAY_PORT              PA_ODR
#define RELAY_BIT               0x08
#define RELAY_TIMER_MULTIPLIER  7

static unsigned int timer;
static bool state;
static bool relayEnable;

/**
 * @brief Configure appropriate bits for GPIO port A, reset local timer
 *  and reset state.
 */
void initRelay()
{
    PA_DDR |= RELAY_BIT;
    PA_CR1 |= RELAY_BIT;
    timer = 0;
    state = false;
    relayEnable = false;
}

/**
 * @brief Sets state of the relay.
 * @param on - true, off - false
 */
static void setRelay (bool on)
{
    if (on) {
        RELAY_PORT |= RELAY_BIT;
    } else {
        RELAY_PORT &= ~RELAY_BIT;
    }

}

/**
 * @brief Enables relay functionality.
 * @param state
 */
void enableRelay (bool state)
{
    relayEnable = state;
}

/**
 * @brief Returns the functional mode of the relay.
 * @return true - enabled, false - disabled.
 */
bool isRelayEnabled()
{
    return relayEnable;
}

/**
 * @brief This function is being called during timer's interrupt
 *  request so keep it extremely small and fast.
 */
void refreshRelay()
{
    bool mode = getParamById (PARAM_RELAY_MODE);

    if (!isRelayEnabled() ) {
        setRelay (mode);
        return;
    }

    if (state) { // Relay state is enabled
        if (getTemperature() < (getParamById (PARAM_THRESHOLD)
                                - (getParamById (PARAM_RELAY_HYSTERESIS) >> 3) ) ) {
            timer++;

            if ( (getParamById (PARAM_RELAY_DELAY) << RELAY_TIMER_MULTIPLIER) < timer) {
                state = false;
                setRelay (!mode);
            } else {
                setRelay (mode);
            }
        } else {
            timer = 0;
            setRelay (mode);
        }
    } else { // Relay state is disabled
        if (getTemperature() > (getParamById (PARAM_THRESHOLD)
                                + (getParamById (PARAM_RELAY_HYSTERESIS) >> 3) ) ) {
            timer++;

            if ( (getParamById (PARAM_RELAY_DELAY) << RELAY_TIMER_MULTIPLIER) < timer) {
                state = true;
                setRelay (mode);
            } else {
                setRelay (!mode);
            }
        } else {
            timer = 0;
            setRelay (!mode);
        }
    }
}
