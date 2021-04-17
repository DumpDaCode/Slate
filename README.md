# Slate
![Screenshot from 2021-04-16 22-51-36](https://user-images.githubusercontent.com/56605804/115114957-84bcf000-9faf-11eb-822a-8a20e3850275.png) 
![Screenshot from 2021-04-17 17-47-05](https://user-images.githubusercontent.com/56605804/115114961-88e90d80-9faf-11eb-9100-c4725de739e8.png)

This is one of my hobby project. It works on linux only and requires gtk+3.24.20 and above.
**Compile it from src/ directory by using _make_ and then run it by ./slate**
It is a simple paint app that has the following features: 

1. 6 Basic tools; pencil, line, curve, circle, bucket fill, eraser.
2. Supports foreground and background color options.
3. Supports undo (upto depth 10) and redo (depth 1 with respect to undo)
4. Responds to ctrl+w, ctrl+y, ctrl+z.
5. Right clicking on the sheet undoes the previous drawing with respect to current tool. (e.g. if eraser tool is selected then this action results in clearing and resetting of background while for rest of the tools it is undo only)
6. Different Cursor for different tool selected. 
7. 3 Different sheet sizes; 400 X 500, 600 X 500, 800 X 600 and 1000 X 600.
8. 3 sizes for tip; 1px 3px 5px.

In future, I would like to add more features to it. Any kind of suggestions are welcomed.
