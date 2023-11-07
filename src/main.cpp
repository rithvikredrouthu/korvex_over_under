#include "main.h"
#include "EZ-Template/auton.hpp"
#include "EZ-Template/sdcard.hpp"
#include "autons.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"

#include "autoSelect/selection.h"
// #include "basicop.cpp"

// Chassis constructor
Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {-chassis_lf_port, -chassis_lm_port, -chassis_lb_port}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{chassis_rf_port, chassis_rm_port, chassis_rb_port}

  // IMU Port
  ,imu_port

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,2.75

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,48.0/36.0

  // Uncomment if using tracking wheels
  /*
  // Left Tracking Wheel Ports (negative port will reverse it!)
  // ,{1, 2} // 3 wire encoder
  // ,8 // Rotation sensor

  // Right Tracking Wheel Ports (negative port will reverse it!)
  // ,{-3, -4} // 3 wire encoder
  // ,-9 // Rotation sensor
  */

  // Uncomment if tracking wheels are plugged into a 3 wire expander
  // 3 Wire Port Expander Smart Port
  // ,1
);



/** 
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
  imu.reset();
	std::cout << pros::millis() << ": calibrating imu..." << std::endl;

	lv_theme_t *th = lv_theme_alien_init(360, NULL); //Set a HUE value and keep font default RED
	lv_theme_set_current(th);

	std::cout << pros::millis() << ": generating paths..." << std::endl;
	//generatePaths();
	std::cout << pros::millis() << ": finished generating paths..." << std::endl;
	
	std::cout << pros::millis() << ": finished creating gui!" << std::endl;
	while (imu.is_calibrating() and pros::millis() < 5000)
	{
		pros::delay(10);
	}
	if (pros::millis() < 5000) std::cout << pros::millis() << ": finished calibrating!" << std::endl;
	else std::cout << pros::millis() << ": calibration failed, moving on" << std::endl;

	std::cout << pros::millis() << ": cata: " << cata.get_temperature() << std::endl;
	std::cout << pros::millis() << ": intake: " << intake.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisLF: " << chassis_lf.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisLM: " << chassis_lm.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisLB: " << chassis_lb.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisRF: " << chassis_rf.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisRM: " << chassis_rm.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisRB: " << chassis_rb.get_temperature() << std::endl;


  // Initialize chassis and auton selector
  default_constants();
  modified_exit_condition();
  // one_mogo_constants();
  // two_mogo_constants();

  pros::ADIDigitalOut left_wing(left_wing_port, false);
	pros::ADIDigitalOut right_endgame(right_endgame_port, false);
  pros::ADIDigitalOut boost(boost_port, false);

  chassis.initialize();
  selector::init();
}



/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	pros::ADIDigitalOut left_wing(left_wing_port, false);
	pros::ADIDigitalOut right_endgame(right_endgame_port, false);
  pros::ADIDigitalOut boost(boost_port, false);
}

pros::Task tcatapult(threadingCatapult, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
pros::Task tchassis(threadingChassis, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
pros::Task tintake(threadingIntake, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");


/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.Ga
 */
void competition_initialize() {
  // . . .
}


/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

  chassis.reset_pid_targets(); // Resets PID targets to 0
  chassis.reset_gyro(); // Reset gyro position to 0
  chassis.reset_drive_sensor(); // Reset drive sensors to 0
  chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.

	intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  cata.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  tchassis.suspend();
  tintake.suspend();

  // Auton Selector
  if (selector::auton == 0){ 
    skills();
  } else if (selector::auton == 1){ 
    near();
  } else if (selector::auton == 2){ 
    far();
  } else if (selector::auton == 3){ 
    awp();
  } else if (selector::auton == -1){ 
    near();
  } else if (selector::auton == -2){ 
    far();
  } else if (selector::auton == -3){ 
    awp();
  }

}



/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

  chassis.set_drive_brake(pros::E_MOTOR_BRAKE_COAST);
  intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  cata.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  tchassis.resume();
  tintake.resume();
  while (true) {

    // chassis.tank(); // Tank control
    // chassis.arcade_standard(ez::SPLIT); // Standard split arcade
    // chassis.arcade_standard(ez::SINGLE); // Standard single arcade
    // chassis.arcade_flipped(ez::SPLIT); // Flipped split arcade
    // chassis.arcade_flipped(ez::SINGLE); // Flipped single arcade


    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) left_wing.set_value(true);
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) right_endgame.set_value(true);
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) left_wing.set_value(true); right_endgame.set_value(true);

    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) auton_mode(true); wait(500); auton_mode(false);;

    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
