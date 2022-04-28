# Swept B-Spline surface

The program reads and displays swept surfaces (reading a 2D profile and a 3D sweeping b-spline curve; uniform and non-uniform b-splines are supported) from the following file format:

```
# Profile (2D)
# Degree (Order - 1)
4
# Number of control points
5
# The knot vector (order + num.ctrl.points)
0,0,0,0.20,0.40,0.60,0.80,1,1,1
# The control points (x,y)
-100.0 -30.0
20.0 30.0
40.0 50.0
60.0 70.0
80.0 -25.0
# Sweep (a cubic b-spline) (3D)
# Number of control points
5
# The knot vector
0,0,0,0.25,0.5,0.75,1,1,1
# The control points (x,y,z)
-100.0 -30.0 10.0
20.0 30.0 12.0
40.0 20.0 20.0
60.0 70.0 -10.0
80.0 25.0 -30.0
```

![image](https://user-images.githubusercontent.com/4311278/165715572-38593eb9-144f-462e-9fc0-11ac984c0453.png)

![image](https://user-images.githubusercontent.com/4311278/165715606-a63ca764-531f-4404-9c3b-b0bbb7a4578d.png)


