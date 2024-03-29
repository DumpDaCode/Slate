# Slate
![Screenshot from 2021-04-16 22-51-36](https://user-images.githubusercontent.com/56605804/115114957-84bcf000-9faf-11eb-822a-8a20e3850275.png) 
![Screenshot from 2021-04-17 17-47-05](https://user-images.githubusercontent.com/56605804/115114961-88e90d80-9faf-11eb-9100-c4725de739e8.png)

This is one of my hobby project. It requires gtk+3.24.20 and glib-2.0.

If you want to compile it on Windows:
1. Install msys2.
2. Add mingw64's bin folder to PATH in environment variables.
3. Open a mingw64 terminal and run install_windows.sh in scripts/
4. Run application from release folder.

If you want to compile it on Linux:  
1. Run install.sh in scripts/
2. Run application from release folder.
 
It is a simple paint app that has the following features: 

1. 6 Basic tools; pencil, line, curve, circle, bucket fill, eraser.
2. Supports foreground and background color options.
3. Supports undo (upto depth 10) and redo (depth 1 with respect to undo)
4. Responds to ctrl+w, ctrl+y, ctrl+z.
5. Different Cursor for different tool selected. 
6. 3 Different sheet sizes; 400 X 500, 600 X 500, 800 X 600 and 1000 X 600.
7. 3 sizes for tip; 1px 3px 5px.

## How to use tools

1. Click and drag for line, circle, pencil and release to set it. 
2. Click and drag to first draw a line and then again click and drag to set a control point for a curve.
3. Right click (all tools except eraser) to undo.
4. Right click (while eraser is selected) causes re setting of background.

In future, I would like to add more features to it. Any kind of suggestions are welcomed.
