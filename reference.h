#ifndef EX3_HealthPoints_H
#define EX3_HealthPoints_H

#include <stdbool.h>
#include <iostream>



class HealthPoints {
public:

    class InvalidArgument{};

     /*
    *
    * C'tor of the healthPoints class
    * 
    * @param maxHP - the maximum HP the player can have, default value is 100
    * @return 
    *        A new instance of a health points
    */
    HealthPoints(int MaxHp = 100);

    

     /*
    * -= operator
    */          
    HealthPoints& operator-=(const int number);

     /*
    * - (minus) operator
    */
    HealthPoints& operator+=(const int number);


     /*
    * taking the default c'tor d'tor and assignment
    */
    HealthPoints& operator=(const HealthPoints& other) = default;
    ~HealthPoints() = default;
    HealthPoints(const HealthPoints&) = default;





private:
    int m_maxHP;
    int m_currentHP;

    /*
    * boolean operators
    */
    friend bool operator>=(HealthPoints healthPoints1, HealthPoints healthPoints2);

    /*
    * print operator
    */
    friend std::ostream& operator<<(std::ostream& os, const HealthPoints& hp);
};

    /*
    * + operator
    */
    HealthPoints operator+(int number, const HealthPoints& healthPoints);
    HealthPoints operator+(const HealthPoints& healthPoints, int number);

     /*
    * - (minus) operator
    */
    HealthPoints operator-(const HealthPoints& healthPoints, int number);


     /*
    * boolean operators
    */
    bool operator>(HealthPoints healthPoints1, HealthPoints healthPoints2);
    bool operator<=(HealthPoints healthPoints1, HealthPoints healthPoints2);
    bool operator<(HealthPoints healthPoints1, HealthPoints healthPoints2);
    bool operator==(HealthPoints healthPoints1, HealthPoints healthPoints2);
    bool operator!=(HealthPoints healthPoints1, HealthPoints healthPoints2);



#endif //EX3_HealthPoints_H