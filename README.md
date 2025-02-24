# Switchboard (TwoWayArrowStep)

The 2-way Switchboard from 3D Land/3D World that moves on a rail. The model was created by VTXG.

## obj_args

|obj_arg|Name|Type|Default|Description|
|-------|----|----|-------|-----------|
|obj_arg0|Maximum Speed|float|30.0f|The maximum speed that the arrow platform reaches (measured in units/frame).|
|obj_arg1|Acceleration|float|1.0f|The acceleration at which rate the platform speeds up and slows down (measured in units/frame).|

## Switches

|Switch|Type|Behavior|
|------|----|--------|
|SW_A|Read|If turned off, the platform will be deactivated until the switch is turned on.|
|SW_B|Write|Turns on while the player stands on the platform.|