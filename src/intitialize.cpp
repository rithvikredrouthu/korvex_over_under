#include "main.h"
#include "api.h"

pros::Motor chassis_lf(chassis_lf_port, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassis_lm(chassis_lm_port, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassis_lb(chassis_lb_port, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassis_rf(chassis_rf_port, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassis_rm(chassis_rm_port, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor chassis_rb(chassis_rb_port, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor intake(intake_port, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor cata(cata_port, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);


pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalOut left_wing(left_wing_port, false);
pros::ADIDigitalOut right_endgame(right_endgame_port, false);
pros::ADIDigitalOut boost(boost_port, false);

pros::Imu imu(imu_port);


pros::ADIDigitalIn limit_switch(limit_switch_port);
