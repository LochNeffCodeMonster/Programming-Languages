       program attack
       
       integer lift, airD, vel, wingS, cruise, weight, altInp, sum
       parameter (cruise = 400, wingS = 1140, weight = 52500)
       
c This is an array of various altitudes (feet)
       real alt(21)
       alt (1:21) = (/ 0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 
       +   	15000, 20000, 25000, 30000, 35000, 36089, 40000, 45000, 50000, 55000 /)
       
c This is an array of various densities (d)
       real den(21)
       den (1:21) = (/ 0.002377, 0.002308, 0.002241, 0.002175, 0.002111, 0.002048, 0.001987,
       +   0.001927, 0.001868, 0.001811, 0.001755, 0.001496, 0.001266, 0.001065, 0.000889, 
       +   0.000737, 0.000706, 0.000585, 0.000460 0.000362, 0.000285 /)

c This array contains (altitude, density)
       real arr1(21,21)

c This array contains (altitude:pitch_angle:lift_value)
       real arr2(21,21,1)

c This array contains Coefficient of Lift (CL) for various Angle of Attack or (Pitch Angle)
       real arr3(21,21)

c Receive input from the user
       write(*,*) 'Enter the intended altitude of the Nighthawk:'
       read(*,*) altInp
       
c Function for calculating Coefficient of Lift (CL)
       real function CL(a)
       int a
       if (a .LT. 16) then
          CL = (0.080625 * a) + 0.16  
       else
          CL = (-0.009874372 * (a**2)) + (0.3721357 * a) - (1.974673)
       endif
       
       return
       end
       
       
       
       stop
       end	  