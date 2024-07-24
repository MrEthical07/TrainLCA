# Terminal Based Train Ticket Booking project 
A C program to reserve train tickets with following functionality:
    1)User System: 
        i)Login
        ii)signup
        iii)Reset Password
    2)Admin Controls
        i)add new trains
        ii)add delayed trains
        iii)see delayed trains
    3)Tickets reservation:	
    	i)With option to select preferred class, compartment,seat type (seater or sleeper) and number of seats
    	ii)With payment system.(with payment failure handling case).											 
    	iii)Once reserved, get PNR.
    4)Get Information of reservations (By entering PNR):
    	i)To know in which compartment, class seat is reserved.
    	ii)To know if ticket is confirmed or in waiting list.
    	iii)Get reservation id or referrence number of the reservation. 
    	iv)To know all about train (max speed, total dist to be travelled )
        V)Option to print tickets in PDF format
    5)Checking Train Status:
    	i)To check delayed trains
    6)Option to cancel tickets:
    	i)option to cancel ticket.
    	ii)with proper refund messages.
     
# ***ADDITIONAL FEATURES***
    
    1)QR code payment format
    2)User profile update
    3)Admin panel
    4)ability to cancel tickets or go back to main menu at any step
    5)Proper error handling.
    6)proper user navigation allowing a smoother program experience.
    7)Proper PDF generation.
    8)Proper user interface allowing user to perform all actions without having to restart the program.
    9)Menu Based program.
   
# *****CAUTIONS*****
    1)Input should be of the type that is asked.
    2)All input should be in lowercase (unless stated otherwise and as per choice for name and other user details).

# Input Required:
    as per the program flow and user wishes.

# Algorithms used (module use):
    1)External library:
        i)qrcodegen.h
        ii)pdfgen.h
    2)Internal header files:
        i)stdio.h
        ii)stdlib.h
        iii)conio.h
        iv)string.h
        v)time.h
        vi)windows.h
        vii)ctype.h
