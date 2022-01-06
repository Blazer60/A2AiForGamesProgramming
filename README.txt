Q-Learning | A2 AI Game Programming Assignment


A Q-Learning algorithm that uses sensory data to navigate and unknown maze.


Project Dependencies
    The project uses GLFW, glm and imgui. All of these can be found in the [vendor] folder.

    The project requires:
    - C++ 17 or greater
    - OpenGl ver 4.6 or greater. (older versions may work but have not been tested).
    - Windows 10 or 11
    - x64 / amd64 compiled using msvc.
    - CMake 3.20 (Recommended) (3.14 works with Visual Studio although not fully tested.)

    When building the program make sure that the .exe file is one folder above the root directory.
    E.g.: source-directory/some-directory. Relative path files have been used for ease of programming.
    This is only a benchmark rather than a fully shippable product. It needs to use things located in
    the [res] folder.

    When trying to run the program, a log file will be generated that can be used to help debugging.

How to use
    **Warning:** While an AI is training or being tested, values can be changed.
    This is not intended behaviour. Changing values out of their slider range can result in a crash.
    Check the logs file to see if there are any problems.

    General
    - Ctrl+Click / double click - change a value by typing it in.
    - Colour Settings - change the colour of the specified element.
    - The logs will show you if anything went wrong. In the event of a crash, logs.txt should be created next to the .exe.

    There are four main options at the top:
    - A* Pathfinding
    - Train AI
    - Run AI
    - Test AI

    You can select one to change to that mode.

    A* Pathfinding
        Select one of the mazes in the maze selection. It will be loaded and A* will show the path as well
        as the nodes it expanded. The logs will tell you if the end cannot be reached.
        You can change the position of the start and the finish with the sliders at the top.

    Train AI
        Adjust any of the sliders and watch the AI train. Selecting an AI will get the info about it but
        not override it when you generate a new AI. You can also select a maze at the bottom that you want
        to train on.

    Run AI
        Like A* pathfinding, you can see what path the AI came up with for the desired start and finish.
        You can also change the maze to see how well the AI performs.

    Test AI
        Testing the AI will compare it to A*. The text file generate can be imported to excel. The results
        are in this order: A* Path size | A* Time (microseconds) | AI Path Size | AI Time (microseconds).
