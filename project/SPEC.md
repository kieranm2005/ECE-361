# SPEC.md: project 19 server room cooling

Device type D: monitor and logger. Key K4, second structure S3, report Q6.

## 1. States

The four states are given by the project sheet.

| State | Meaning |
|---|---|
| NORMAL | within limits (initial state) |
| ELEVATED | warm, or humidity outside the band |
| CRITICAL | hot |
| FAILOVER | backup cooling requested, held until acknowledged |

## 2. Inputs and outputs

`iom361_initialize(1, 3, &rc)`: 1 switch, 3 LEDs.

| Bit | Switch | Kind | Meaning |
|---|---|---|---|
| 0 | ACK | press | Acknowledges FAILOVER |

| Bit | LED | On means |
|---|---|---|
| 0 | WARN | elevated |
| 1 | ALARM | critical or failover |
| 2 | BACKUP_REQUEST | backup cooling requested |

The RGB LED shows the state: one distinct color per state.

| State | RGB color |
|---|---|
| NORMAL | green |
| ELEVATED | yellow |
| CRITICAL | red |
| FAILOVER | blue |

## 3. Events

The events are the things the device reacts to. Each one is a level or a
press. A level event is re-evaluated every tick; a press event fires once,
on the 0-to-1 change of its switch bit.

| Event | Source | When it fires |
|---|---|---|
| EV_TEMP_OK | temperature reading | temp < 26.0 C |
| EV_TEMP_ELEVATED | temperature reading | temp >= 27.0 C and temp < 31.0 C |
| EV_TEMP_CRITICAL | temperature reading | temp >= 32.0 C |
| EV_TEMP_COOLED | temperature reading | temp < 26.0 C while CRITICAL or FAILOVER |
| EV_RH_OK | humidity reading | 20.0 <= rh <= 80.0 |
| EV_RH_OUT | humidity reading | rh < 20.0 or rh > 80.0 |
| EV_CRITICAL_60 | the FSM's own timer | 60 consecutive ticks in CRITICAL |
| EV_ACK | switch register bit 0, 0-to-1 | each press of ACK |

## 4. Transition table

HW 5