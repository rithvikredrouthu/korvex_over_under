#include "main.h"
#include "pros/rtos.hpp"
#include <fstream>

void spinIntake() {
  intake.move_velocity(600);
}

void outtake() {
  intake.move_velocity(-600);
}

void spinRollers() {
  intake.move_velocity(-400);
}

void stopIntake() {
  intake.move_velocity(0);
}

void wait(int sec) {
  pros::delay(sec);
}

void endgame() {
  left_wing.set_value(true);
  right_endgame.set_value(true);
}

void auton_mode(bool mode) {
  boost.set_value(mode);
}

void shoot() {
  shooting = true;
}


