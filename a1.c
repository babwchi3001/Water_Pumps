//------------------------------------------------------------------------------
// a1.c
//
// In this program, we obtain the number of pumps that are needed, so that the
// water can reach the target with at least 6 bar of pressure.
//
//
// Group: Lena Klambauer
//
// Author: 11937605
//------------------------------------------------------------------------------
//
#include <stdio.h>
#include <math.h>
#define TYPE_1 1
#define TYPE_2 2
#define TYPE_3 3
#define INVALID_VALUE 0
#define SUCCESS 0

// forward declarations
int getInput(int type);
double lamnda(int water_pressure);
double waterAngle(int hor_x, int ver_y);
void pumps(int horizontal_x, int vertical_y, double lamnda_const,
           double alpha_angle);

//------------------------------------------------------------------------------
///
/// In the main function. We get the needed inputs in order to calculate the
/// number of pumps. So that the pressure wont go under 6 bar.
///
/// @return SUCCESS meaning the code ended without a problem
//
int main()
{
  int water_pressure = getInput(TYPE_1);
  int horizontal_x = getInput(TYPE_2);
  int vertical_y = getInput(TYPE_3);
  double lamnda_const = lamnda(water_pressure);
  double alpha_angle = waterAngle(horizontal_x, vertical_y);

  printf("\nZiel: (%d, %d)",horizontal_x, vertical_y);
  printf("\nNeigung [rad]: %.4lf", alpha_angle);
  printf("\nDurchfluss [l/min]: %d", water_pressure);
  printf("\nReibungsbeiwert [bar/m]: %.4lf\n", lamnda_const);

  pumps(horizontal_x, vertical_y, lamnda_const, alpha_angle);

  return SUCCESS;
}

//------------------------------------------------------------------------------
///
/// In the getInput function, we get an input from the console using getchar().
/// We parse that input, which is a character type and convert it to integer.
/// Depending on the value of the "type" parameter, the function will print the
/// appropriate outputs in the console.
///
/// @param type used to decide the workflow of the function
///
/// @return sum the converted input
//
int getInput(int type)
{
  int max_wp = 1200;
  int min_wp = 100;
  int char_to_int = 48;
  int sum = 0;
  int zero = 48;
  int nine = 57;
  int decimal = 10;

  int loop = 1;
  while(loop)
  {
    if(type == TYPE_1)
      printf("Erforderlicher Durchfluss [l/min]: ");
    else if(type == TYPE_2)
      printf("Horizontale Distanz [m]: ");
    else
      printf("Vertikale Distanz [m]: ");

    sum = 0;
    int wrong_input = 0;
    int input_char = 0;
    while(( input_char = getchar()) != '\n')
    {
      if(input_char < zero || input_char > nine)
        wrong_input = 1;
      if(wrong_input)
        continue;

      // calculating digit value
      int digit_value = input_char-char_to_int;
      sum *= decimal;
      sum += digit_value;
    }
    loop = 0;
    if(type == TYPE_1)
    {
      if (sum > max_wp || sum < min_wp)
        wrong_input = 1;
    }
    else
    {
      if (sum <= INVALID_VALUE)
        wrong_input = 1;
    }
    if(wrong_input)
    {
      if(type == TYPE_1)
        printf("Invalide Eingabe! Der Durchfluss muss mindestens "
               "100 l/min und maximal 1200 l/min betragen.\n");
      else
        printf("Invalide Eingabe!\n");
      loop = 1;
    }
  }
  return sum;
}

//------------------------------------------------------------------------------
///
/// In the lamnda function, we get the values of the Lambda constant based
/// from the water pressure that we inputted before.
///
/// @param water_pressure integer value of water pressure
///
/// @return lamnda_const the lambda corresponding to the pressure
//
double lamnda(int water_pressure)
{
  double lamnda_const = 0.0;

  if(water_pressure < 200)
    lamnda_const = 0.001;
  else if(water_pressure > 200 && water_pressure <= 400)
    lamnda_const = 0.0025;
  else if(water_pressure > 400 && water_pressure <= 600)
    lamnda_const = 0.005;
  else if(water_pressure > 600 && water_pressure <= 800)
    lamnda_const = 0.01;
  else if(water_pressure > 800 && water_pressure <= 1000)
    lamnda_const = 0.015;
  else if( water_pressure > 1000 && water_pressure <= 1200)
    lamnda_const = 0.025;

  return lamnda_const;
}

//------------------------------------------------------------------------------
///
/// In the waterAngle function, we calculate the alpha angle.
///
/// @param hor_x x value
/// @param ver_y y value
///
/// @return alpha_angle the calculated angle
//
double waterAngle(int hor_x, int ver_y)
{
  double alpha_angle = atan((double)ver_y / (double)hor_x);
  return alpha_angle;
}

//------------------------------------------------------------------------------
///
/// In the pumps function, we calculate calculate the number of pumps.
/// So that the pressure wont go under 6 bar. And we print in the console the
/// required outputs.
///
/// @param horizontal_x x value
/// @param vertical_y y value
/// @param lamnda_const lambda value
/// @param alpha_angle the angle
///
/// @return none
//
void pumps(int horizontal_x, int vertical_y, double lamnda_const,
           double alpha_angle)
{
  double delta_horizontal = 0;
  double delta_vertical = 0;
  double pressure_loss_meter = 0.1;
  double max_pressure_loss = 8.5;
  double pump_pressure = 10.0;
  double pressure_limit = 6;

  int pump_nr = 0;
  int loop = 1;
  while(loop)
  {
    double new_delta_vertical = vertical_y - delta_vertical;

    double pressure_on_target = pump_pressure -
        (pressure_loss_meter * new_delta_vertical +
        lamnda_const * (new_delta_vertical/sin(alpha_angle)));

    if(pressure_on_target >= pressure_limit)
    {
      printf("Austrittsdruck Zielpunkt [bar]: %.4lf\n",
             pressure_on_target);
      loop = 0;
      continue;
    }

    double const_vertical = max_pressure_loss /
        (pressure_loss_meter + (lamnda_const / sin(alpha_angle)));
    double const_horizontal = const_vertical / tan(alpha_angle);

    delta_vertical += const_vertical;
    delta_horizontal += const_horizontal;

    if(delta_vertical >= vertical_y)
      delta_vertical = vertical_y;

    if(delta_horizontal >= horizontal_x)
      delta_horizontal = horizontal_x;

    pump_nr++;

    printf("  Pumpe%d: (%.2lf, %.2lf)\n", pump_nr, delta_horizontal,
           delta_vertical);
  }
}
