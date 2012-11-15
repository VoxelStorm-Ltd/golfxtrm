! Final Project for Fortran - Andrew Werner - March 13, 2007 - Flight of a Golf Ball
! Design a project that allows you to investigate a physics problem in depth throw the use of a Fortran program.
! The problem I chose to investigate is the flight path of a golf ball.
! Assumptions being used(made)
! *The golf shot is being made at standard ambient temperature and pressure of 25 degrees C (70 F) and 100KPa.
! That the average driver club head mass is 200 grams.
! The drag coefficient of a golf ball is 0.21
! The lift coefficient of a golf ball is 0.14
! The radius of a golf ball is 21.335 mm
! The mass of a golf ball is 45.93 grams
! The coefficient of restitution of the club being used is at the USGA limit of 0.83
! The average mass of a driver club head is 200 grams
! *The golf ball maintains a constant spin rate throughout the flight of the shot.
! *The launch angle of the shot is 3 degrees more than the loft of the driver.
! *The rotation of the golf ball is only in the x-direction and does not have any (what would be) z-components
! That the force of gravity is a constant, would actually depend upon elevation => distance from the center of the earth.
PROGRAM golf_ball
IMPLICIT NONE
REAL(KIND=8)::launch_angle,swing_speed,loft_club,ball_speed,delta_t=.001,lC=0.14,dC=0.21,air_density
REAL(KIND=8)::mass_clubhead, drag_force, lift_force, g=9.8,radius,ball_mass
REAL(KIND=8)::area_cross, swing_speed_mph, launch_angle_deg, last_point
REAL,DIMENSION(100000)::a_drag,a_lift,a_x,v_x,x_m,a_y,v_y,y_m,theta,V,y_yards,x_yards
INTEGER::i
INTEGER:: PGBEG, IER, max_distance_m, max_distance_yds
!LOGICAL:: y_y
REAL(KIND=10)::one=1, zero=0, n_one=(-1), pi !used to determine the value of pi
pi = (ACOS(n_one))
!Launch angle = launch angle of the golf ball in relation to the ground, figure from loft of club, in degrees
!swing_speed = the speed of the particular golfer, prompt for mph then convert to m/s
!loft_club = the loft of the club being used in degrees
!ball_speed = the speed of the golf ball in m/s
!t = time in seconds
!delta_t = the increment of time, being used to create a number of heights at certain t's
!lC = Lift coefficient
!dC = drag coefficient
!air_density = density of the air being traveled through, in g/m^3
!radius = golf ball radius in mm
!ball_mass = mass of the golf ball in grams
!mass_clubhead = mass of the club head of the golf club
!area_cross = the cross sectional area of a golf ball, pi*r^2
!drag_force = the drag force felt by the golf ball
!lift_force = the lift force felt by the golf ball
PRINT *, 'Welcome to the driver distance figure-outer'
PRINt *, ' The driver distance figure outer will ask you for your swing speed
along with the loft of your driver,'
PRINT *, ' and in return figure out how far your shot will travel.'
PRINT *, "So let's get started."
PRINT *,' '
PRINT *, 'What is your swing speed in mph?'
READ *, swing_speed_mph
PRINT *, ' '
PRINT *, 'What is the loft of you driver (in degrees)?'
READ *, loft_club
!To decrease complications in this project, I will use the case of standard
temperature and pressure (25 C and 100KPA)
!At standard ambient temperature and pressure the density of air is 1.168 kg/(m^3 => 1168 g/(m^3)
air_density = 1.168 !kg/m^3
!The properties of the golf ball of radius and mass will be those as defined by the USGA in the rules of golf.
!The minimum diameter of a golf ball is specified as 42.67 mm, radius =>21.335 mm.
radius = .021335 !m
!The maximum mass of the golf ball is specified as 45.93 grams by the USGA in the rules of golf.
ball_mass = .04593 !kg
!The average mass of the driver club head is said to be 200 grams.
mass_clubhead = .200 !kg
!Here I am going to convert the ball_speed_mph from mph into meters/second
!5280 ft = 1 mile, 3600 secs = 1 hour, .304800610 m = 1 ft
swing_speed = swing_speed_mph * (5280./3600.) * .30480061
!Here is the relationship between the swing speed and the resulting ball speed.
!By the conservation of momentum, we know m1v1 = m2v2
!ball_speed = (swing_speed * ball_mass) / clubhead_mass This equation would
seem to be of the correct form,
!However one of the hot topics in golf is the coefficient restitution,
!which is a measure of the efficiency of the kinetic energy transfer between club and ball
! The USGA has also put a limit on this attribute of the golf club, which is
0.83
! Applying this attribute to our conservation of momentum equation we get
ball_speed = swing_speed * ( (1.+0.83)/(1.0+ (ball_mass/mass_clubhead) ) )
!For the launch angle, I'm going to take into consideration that most golfers catch the ball on the slight
! upswing with their driver, adding a few degrees on top of their club head loft for their launch angle
launch_angle_deg = loft_club + 3.0
launch_angle = (launch_angle_deg / 180) * pi !Converting the launch angle into
radians in order to use intrinsic functions.
!In order to be congruent with the forms of equations I am using from my derivations I will define
! the launch angle to = theta, and the angle of (90 - theta) to = phi
!theta = launch_angle
!phi = 90. - theta
!For the cross_sectional area of the golf ball, we get the form of
area_cross = pi * (radius**2.)
!For the value of pi, above I used a method from one of my previous homeworks
!Now that we have the initial velocity of the ball and the launch angle, we can go to town on the
! equations that determine the flight of the ball
! The forces on the golf ball are the drag force, the lift force, and the force of gravity.
! While the force of gravity is always downward (-y), the drag force is always parallel to the direction of motion,
! while the lift force is always perpendicular to the direction of motion, and in the case of a golf ball,
! since there is always backspin imparted on the golf ball the lift force will always be upward (+y)
!From these thoughts we can determine that the both the lift force and the drag force will have horizontal and
!vertical components.
!Using the form of F=ma, and Ftotal = Fdrag + Flift + mg = ma, we can come up with the forms of
! a = (Flift + Fdrag + mg) / ball_mass
!Separating this form of the acceleration of the golf ball into x and y components yields
! x direction: a_x = ( lift_force*COS(phi) + drag_force*SIN(theta) ) / ball_mass
! y direction: a_y = ( lift_force*SIN(phi) + drag_force*COS(theta) + ball_mass*g) / ball_mass
! After taking two derivatives of y''=a, y'=at + v, y=(1/2)at^2 + vt + y, and the same for x, I come up with
! x = .5*a_x*(t**2) + t*(ball_speed*SIN(theta))
! y = .5*a_y*(t**2) + t*(ball_speed*COS(theta))
!As equations of the y and x ball positions as a function of time (and currently theta)
!Now for the drag and lift forces.
!lift_force = .5*air_density*(ball_speed**2)*cross_area*lC
!drag_force = .5*air_density*(ball_speed**2)*cross_area*dC
!lift_force = .5*air_density*(ball_speed**2)*area_cross*lC
!drag_force = .5*air_density*(ball_speed**2)*area_cross*dC
!a_x = ( -lift_force*COS(phi) - drag_force*SIN(theta) ) / ball_mass
!a_y = ( lift_force*SIN(phi) - drag_force*COS(theta) - ball_mass*g ) / ball_mass theta(1) = launch_angle
!With V(i) being the magnitude of the velocity of the golf ball, we can us V = v_x^2 + v_y^2 for the magnitude
v_x(1) = ball_speed * COS(launch_angle)
v_y(1) = ball_speed * SIN(launch_angle)
V(1) = SQRT( v_x(1)**2. + v_y(1)**2. )
a_x(1) = 0
a_y(1) = 0
x_m(1) = 0
y_m(1) = 0
a_lift(1) = 0
a_drag(1) = 0
!PRINT *, swing_speed, ball_speed
!PRINT *, a_y(1:5)
!PRINT *, v_y(1:5)
!PRINT *, y(1:5)
!PRINT *,' '
!PRINT *, air_density, area_cross, dC, ball_mass, lC, V(1)
DO i=2,100000
a_drag(i) = ( (.5*air_density*area_cross*dC)/ball_mass) * (V(i-1)**2.)
a_lift(i) = ( (.5*air_density*area_cross*lC)/ball_mass) * (V(i-1)**2.)
a_x(i) = a_drag(i)*(-COS(theta(i))) + a_lift(i)*(-SIN(theta(i-1)))
v_x(i) = v_x(i-1) + a_x(i)*delta_t
x_m(i) = x_m(i-1) + v_x(I)*delta_t
x_yards(i) = x_m(i)*(1./0.9144)
a_y(i) = -g + a_drag(i)*(-SIN(theta(i))) + a_lift(i)*(COS(theta(i-1)))
v_y(i) = v_y(i-1) + a_y(i)*delta_t
y_m(i) = y_m(i-1) + v_y(I)*delta_t
y_yards(i) = y_m(i)*(1./0.9144) !There are 0.9144 meters in a yard.
V(i) = SQRT( v_x(i)**2. + v_y(i)**2. )
theta(i) = ATAN( v_y(i) / v_x(i) )
last_point = 0
IF ( (y_m(i)<0) .AND. (last_point == 0)) THEN
max_distance_m = x_m(i)
EXIT
END IF
END DO
!PRINT *, a_drag(1:5) Used for debugging the program.
!PRINT *, a_lift(1:5)
!PRINT *, a_x(1:5)
!PRINT *, v_x(1:5)
!PRINT *, x(1:5)
!IER = PGBEG(0,'ballflight.ps/PS',1,1)
IER = PGBEG(0,'/xserve',1,1) !Naming the graph, selecting the graphics device.
IF (IER.NE.1) STOP
CALL
PGENV(0,MAXVAL(REAL(x_yards(1:100000))),0,2*MAXVAL(REAL(y_yards(1:10000))),0,1)
!Defining plot scales according to the min and max array values.
CALL PGLAB('Distance (yds)','Height (yds)','Flight Path of a Golf Ball')
!Creating labels for the graph
CALL PGPT(20000, REAL(x_yards(1:20000)), REAL(y_yards(1:20000)),20)
!Creating the point markers in the graph.
CALL PGEND
max_distance_yds = max_distance_m * (1./ 0.9144)
!There are 0.9144 meters in a yard.
PRINT *,' '
PRINT *, 'The length of you shot was', max_distance_yds,'yards'
PRINT *, 'Wow, great shot!'
PRINT *, ' '
PRINT *, 'Have a great day!'
END PROGRAM golf_ball
