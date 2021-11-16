#include "minCircle.h"

//calculate the distance of two points by the regular formula.
float dist(Point x1, Point x2) {
    return sqrt(pow((x1.x-x2.x),2)+pow((x1.y-x2.y),2));
}
//finds the minimum circle between two points.
Circle from2points(Point p, Point r) {
    //the center of this circle is always in the center of the points.
    float centerX = (p.x + r.x) /2.0;
    float centery = (p.y + r.y) /2.0;
    Point center = Point (centerX,centery);
    float radius = dist(center, p);
    return Circle(Point(centerX,centery),radius);
}
//checks if a specific point is in the circle or not.
bool isPointInCircle(Circle c,Point p) {
    //if the distance between the point is smaller than the radius - it's inside the circle.
    if (dist(c.center, p) <= c.radius)
        return true;
    return false;
}
//calculate the circle that is defined by 3 points.
Circle from3Points(Point x , Point y , Point z) {
    //the function calculate two lines of the line bisections and calculate their intersection.
    float m1 = (y.y - x.y) / (y.x - x.x);
    m1 = -1.0 /m1;//the first slope that intersects the circle.
    float n1 = ((x.y + y.y) /2.0) -m1*((x.x + y.x) /2.0); //the n value in the formula y=mx+n of first slope.
    float m2 = (y.y - z.y) / (y.x - z.x);
    m2 = -1.0 /m2;//the second slope that intersects the circle.
    float n2 = ((z.y + y.y) /2.0) -m2*((z.x + y.x) /2.0);//the n value in the formula y=mx+n of second slope.
    float newX = (n2-n1) / (m1-m2);//calculate the x and y of the center using the two lines.
    float newY = m1*newX + n1;
    return Circle(Point(newX,newY), dist(x, Point(newX, newY)));
}
//calculate the min circle for three points.
Circle minForREqual3(vector<Point> r) {
    assert (r.size() ==3);
    //checks all the possibilities for circle by two points and if its containts the
    //third points its the minimum circle- otherwise returns the circle that is made by all the points.
    Circle circle1 = from2points(r[0], r[1]);
    Circle circle2 = from2points(r[0], r[2]);
    Circle circle3 = from2points(r[1], r[2]);
    if (isPointInCircle(circle1,r[2]))
        return circle1;
    if (isPointInCircle(circle2,r[1]))
        return circle2;
    if (isPointInCircle(circle3,r[0]))
        return circle3;
    return from3Points(r[0], r[1], r[2]);

}
//calculate the case of the points vector equals zero.
Circle trivial(vector<Point>& r) {
    int size = r.size();
    switch (size) {
        case 0: //if there are no points there is no circle to return
            return Circle(Point(0,0),0);
        case 1://for one point returns the point with radius 0
            return Circle(r[0],0);
        case 2://for two point go to the needed function
            return from2points(r[0], r[1]);
        default:return minForREqual3(r);//for 3 points go to the needed function.

    }
}
//the function for the minimum point. can't use the main function because of the recursion.
Circle welzl(Point** points, size_t size, vector<Point> r) {
    if (r.size() == 3)//if there are three points in the sides of the circle return the needed circle
        return from3Points(r[0], r[1], r[2]);
    if(size == 0)//if there are no points left return only for the points in the sides.
        return trivial(r);
    Point newPoint = *points[size-1];//deleting point from the vector.
    size--;
    //returns recursively the minimum circle for all the point except the last one
    Circle minCircle = welzl(points, size, r);
    if (isPointInCircle(minCircle,newPoint)) //if the last point that is popped inside the circle return the circle.
        return minCircle;
    r.push_back(newPoint);//if not the point will always be on the side of the circle.
    //calls for this function again while the last point
    // is in the side of the circle and removed from the main points vector.
    return welzl(points, size, r);
}
//the main function that calculates the needed minimum circle.
Circle findMinCircle(Point** points,size_t size) {
    vector<Point> v,pointsVector;
    //calls the first main function with all the points and zero points known on the side of the circle
    return welzl(points, size, v);
}

