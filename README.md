# GAZEBO PADAWAN LEVEL 1

## 1 - PREPARE WORKSPACE
Copypaste all the folders into your catkin workspace and do the following:

`cd <path/to/catkin/workspace>`
`catkin_make`
`source devel/setup.bash`

## 2 - LAUNCHING WORLD + ROBOT
`roslaunch gazebo_worlds gazebo.launch`


----------------------

## Using the docker image:

Copypaste all the folders into your catkin workspace and do the following:

First `./build` the docker image.

Then you should run it and indicate the `workspace`.

`cd <path/to/catkin/workspace>`
`./src/docker/run -ws <path/to/catkin/workspace>`

And then do step 1 and 2.
