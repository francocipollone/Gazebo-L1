# GAZEBO PADAWAN LEVEL 1

## 1 - PREPARE WORKSPACE
Clone all the folders into your catkin workspace and do the following:

`cd <path/to/catkin/workspace>`
`catkin_make`
`source devel/setup.bash`

## 2 - LAUNCHING WORLD + ROBOT
`roslaunch gazebo_worlds gazebo.launch`


----------------------

## Using the docker image:

Clone all the folders into your catkin workspace and do the following:

First `./build` the docker image.

Then you should run it and indicate the `workspace`.

`cd <path/to/catkin/workspace>`
`./src/docker/run -ws <path/to/catkin/workspace>`

And then do step 1 and 2.


## Moving the robot.

The `teleop_twist_keyboard` node is launched when the world is loaded to be able to move the robot using your keyboard.

```
Moving around:
   u    i    o
   j    k    l
   m    ,    .

For Holonomic mode (strafing), hold down the shift key:

   U    I    O
   J    K    L
   M    <    >

t : up (+z)
b : down (-z)

anything else : stop

q/z : increase/decrease max speeds by 10%
w/x : increase/decrease only linear speed by 10%
e/c : increase/decrease only angular speed by 10%
```

Also, `rqt_steering` node is launched, thus you can move the robot using the GUI interface.
