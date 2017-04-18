PROGRAM hello 

    WRITE(*,*) 'Hello World'
    REAL :: points = 100 
    IF (points > 0) THEN
        WRITE(*,*) 'Non negative points'
    ELSE IF (points > 500) THEN
        WRITE(*,*) 'More than 999'
    ELSE
        WRITE(*,*) 'Negative points'
    END IF


    DO WHILE (points > 999)
        WRITE(*,*) 'DANG'
    END DO


    SELECT CASE (points)
        CASE (0)
            WRITE(*,*) "You have no points"
        CASE (1)
            WRITE(*,*) "You have one point"
        CASE DEFAULT
    END SELECT

END PROGRAM 
