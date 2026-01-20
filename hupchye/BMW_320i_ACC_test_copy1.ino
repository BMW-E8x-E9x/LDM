// Required libraries
#include "variant.h"
#include "due_can.h"

CAN_FRAME incoming, id0xAC, id0xB4, id0xD5, id0x193, id0x194, id0x1B4;

byte cruise_min_speed = 0x1E;
byte acc_distance_set;
byte cc_speed;
byte car_speed_current;
byte car_accelerator = 0x21;
byte car_yellow = 0x11;
byte car_red = 0xFA;
byte brake_force_1;
byte brake_force_2;
int brake_force_init = 31999; // 0x7C FF
int brake_force_set = 30460;  // 0x77 00
int brake_force_actual;
int brake_force_control = 31999;
int brake_force_control_old;
int brake_force_control_base = 7;
long object_relativespeed_current;
long object_distance_current;
boolean brake_auto_on = false;
boolean brake_auto_on_latch = false;
boolean brake_cruise_on = false;
boolean transmission_park_neutral;
boolean transmission_drive;
boolean pedal_brake = false;
boolean pedal_accerelator = false;
boolean cruise_cc_on = false;
boolean cruise_cc_cut = false;
boolean cruise_acc_on = false;
boolean cruise_acc_on_low = false;
boolean cruise_button_on = false;
boolean speed_changed = false;
boolean speed_below = false;
boolean acc_distance_changed = false;
boolean object_distance_1 = false;
boolean object_distance_1_mid = false;
boolean object_distance_2 = false;
boolean object_distance_2_mid = false;
boolean object_distance_3 = false;
boolean object_distance_3_mid = false;
boolean object_distance_4 = false;
boolean object_distance_4_mid = false;
unsigned long loop_timer_id0xAC;
unsigned long loop_timer_id0xB4;
unsigned long loop_timer_id0x193;
unsigned long loop_timer_id0x194;
unsigned long loop_timer_id0x1B4;
byte counter_id0xAC = 0;
byte counter_id0xB4 = 0;
byte counter_id0x194_1 = 0;
byte counter_id0x194_2 = 0;
byte counter_id0x1B4 = 0;
byte id0xD5_data_bytes0;
int checksum;
byte id0xD5_data_bytes3 = 0xFF;
byte id0xD5_data_bytes4 = 0xFF;
byte id0x194_data_bytes2;
byte id0x1B4_data_bytes0;
byte id0x1B4_data_bytes1;

void setup() {

// start serial port at 115200 bps: 
  Serial.begin(115200);
  
  // Verify CAN0 and CAN1 initialization, baudrate is 500Kb/s:
  if (Can0.begin(CAN_BPS_500K) &&
	  Can1.begin(CAN_BPS_500K)) {
    Serial.println("CAN initialization ok");
  }
  else {
    Serial.println("CAN initialization (sync) ERROR");
  }
  
  //Both of these lines create a filter on the corresponding CAN device that allows
  //just the one ID we're interested in to get through.
  //The syntax is (mailbox #, ID, mask, extended)
  //You can also leave off the mailbox number: (ID, mask, extended)
  Can0.watchFor();
  //DME
//  Can1.watchFor(0xA8);
//  Can1.watchFor(0xA9);
//  Can1.watchFor(0xAA);
//  Can1.watchFor(0x135);
//  Can1.watchFor(0x1B6);
//  Can1.watchFor(0x1D0);
//  Can1.watchFor(0x200);
  //EGS
//  Can1.watchFor(0xB5);
//  Can1.watchFor(0xBA);
//  Can1.watchFor(0x1A2);
//  Can1.watchFor(0x1D2);
  //CAS
//  Can1.watchFor(0x130);
  //KOMBI
//  Can1.watchFor(0x1B4);
  //IHKA
//  Can1.watchFor(0x1B5);

  Can1.watchFor();
  //DSC
//  Can1.watchFor(0xB6);
//  Can1.watchFor(0xC4);
//  Can1.watchFor(0xC8);
//  Can1.watchFor(0xCE);
//  Can1.watchFor(0xE1);
//  Can1.watchFor(0x194);
//  Can1.watchFor(0x19E);
//  Can1.watchFor(0x1A0);
//  Can1.watchFor(0x1A6);
//  Can1.watchFor(0x1D6);
//  Can1.watchFor(0x2A6);
//  Can1.watchFor(0x2B2);
//  Can1.watchFor(0x31D);
//  Can1.watchFor(0x374);
//  Can1.watchFor(0x4A9);
//  Can1.watchFor(0x5A9);
  //LDM
  //Can1.watchFor(0xBF);
  //Can1.watchFor(0xD5);
  //Can1.watchFor(0x150);
  //Can1.watchFor(0x153);
  //Can1.watchFor(0x15F);
  //Can1.watchFor(0x193);
  //Can1.watchFor(0x49C);
  //Can1.watchFor(0x59C);
  //ACC
//  Can1.watchFor(0x159);
//  Can1.watchFor(0x15C);
//  Can1.watchFor(0x4A1);
  
  test_1();
}

static void test_1(void)
{

  id0xAC.id = 0xAC;
  id0xAC.length = 0x08;
  id0xAC.data.bytes[0] = 0x98;
  id0xAC.data.bytes[1] = 0xF0;
  id0xAC.data.bytes[2] = 0xFF;
  id0xAC.data.bytes[3] = 0xFF;
  id0xAC.data.bytes[4] = 0x00;
  id0xAC.data.bytes[5] = 0x7D;
  id0xAC.data.bytes[6] = 0x00;
  id0xAC.data.bytes[7] = 0x7D;

  id0xB4.id = 0xB4;
  id0xB4.length = 0x08;
  id0xB4.data.bytes[0] = 0x9D;
  id0xB4.data.bytes[1] = 0x00;
  id0xB4.data.bytes[2] = 0x00;
  id0xB4.data.bytes[3] = 0x00;
  id0xB4.data.bytes[4] = 0x7D;
  id0xB4.data.bytes[5] = 0x00;
  id0xB4.data.bytes[6] = 0xF0;
  id0xB4.data.bytes[7] = 0x7A;

  id0xD5.id = 0xD5;
  id0xD5.length = 0x06;
  id0xD5.data.bytes[2] = 0x64;
  id0xD5.data.bytes[5] = 0xF1;

  id0x193.id = 0x193;
  id0x193.length = 0x08;
  id0x193.data.bytes[2] = 0xFC;
  id0x193.data.bytes[3] = 0x01;

  id0x194.id = 0x194;
  id0x194.length = 0x04;
  id0x194.data.bytes[3] = 0xFC;

  acc_distance_set = 0x5C;
  loop_timer_id0xAC = micros();
  loop_timer_id0xB4 = micros();
  loop_timer_id0x194 = micros();
  loop_timer_id0x1B4 = micros();
  
  while (1==1) {

    if (Can1.available() > 0) {
      Can1.read(incoming);
        if (incoming.id != 0x200) {
          if (incoming.id != 0x194) {
            if (incoming.id != 0x19E) {

//0xA8
              if (incoming.id == 0xA8) {
                if (incoming.data.bytes[7] == 0x60 || incoming.data.bytes[7] == 0x62 || incoming.data.bytes[7] == 0x63) {
                  cruise_acc_on_low = false;
                  cruise_acc_on = false;
                  cruise_cc_on = false;
                  cruise_cc_cut = false;
                }
              }

//0xAA
              if (incoming.id == 0xAA) {
                if (incoming.data.bytes[6] == 0x94) {
                  pedal_accerelator = true;
                }
                else {  
                  pedal_accerelator = false;
                }
              }

//0xBA
              if (incoming.id == 0xBA) {
                if (incoming.data.bytes[2] == 0x3F) {
                  transmission_park_neutral = true;
                  transmission_drive = false;
                }
                if (incoming.data.bytes[2] == 0x30 || incoming.data.bytes[2] == 0x31) {
                  transmission_park_neutral = false;
                  transmission_drive = true;
                }
              }

//0xE1
              if (incoming.id == 0xE1) {
                brake_force_1 = incoming.data.bytes[3];
                brake_force_2 = incoming.data.bytes[4];
                brake_force_actual = (brake_force_2 * 256) + brake_force_1;
              }

//0x1B4
              if (incoming.id == 0x1B4) {
                id0x1B4_data_bytes0 = incoming.data.bytes[0];
                id0x1B4_data_bytes1 = incoming.data.bytes[1];
                id0x1B4.data.bytes[2] = incoming.data.bytes[2];
                id0x1B4.data.bytes[3] = incoming.data.bytes[3];
                id0x1B4.data.bytes[4] = incoming.data.bytes[4];
                id0x1B4.data.bytes[5] = incoming.data.bytes[5];
                id0x1B4.data.bytes[6] = incoming.data.bytes[6];
                id0x1B4.data.bytes[7] = incoming.data.bytes[7];
                car_speed_current = incoming.data.bytes[0] + (incoming.data.bytes[1] - 0xD0) * 256;
                if (cruise_acc_on == true && cruise_cc_on == true && car_speed_current / 10 > cc_speed + 3 && cc_speed > 0 && brake_auto_on == false && pedal_accerelator == false) {
                  brake_force_control_base = 1;
                  brake_cruise_on = true;
                }
                else {
                  brake_cruise_on = false;
                }
              }

//0x1D6
              if (incoming.id == 0x1D6) {
                if (incoming.data.bytes[0] == 0xC1) {
                  if (cruise_acc_on == true && acc_distance_set <= 0x5C && acc_distance_changed == false) {
                    acc_distance_set++;
                    id0x193.data.bytes[5] = acc_distance_set;
                    acc_distance_changed = true;
                  }
                }
                else {
                  if (incoming.data.bytes[1] == 0x0D) {
                    if (cruise_acc_on == true && acc_distance_set >= 0x5B && acc_distance_changed == false) {
                      acc_distance_set--;
                      id0x193.data.bytes[5] = acc_distance_set;
                      acc_distance_changed = true;
                    }
                  }
                  else {
                    acc_distance_changed = false;
                  }
                }
              }

              Can0.sendFrame(incoming);
            }

//0x19E
            else {
              Can0.sendFrame(incoming);
              incoming.data.bytes[4] = 0xFE;
              incoming.data.bytes[7] = incoming.data.bytes[7] - 1;
              Can1.sendFrame(incoming);
            }
          }

//0x194
          else {
            id0x194.data.bytes[0] = incoming.data.bytes[0];
            id0x194.data.bytes[1] = incoming.data.bytes[1];
            id0x194_data_bytes2 = incoming.data.bytes[2];
            if (incoming.data.bytes[2] == 0x01 || incoming.data.bytes[2] == 0x04) {
              cruise_button_on = true;
              counter_id0x194_2 = 0;
            }
            else {
              cruise_button_on = false;
              loop_timer_id0x194 = micros();
              counter_id0x194_1 = 0;
              speed_changed = false;
              counter_id0x194_2++;
              if (counter_id0x194_2 > 7) counter_id0x194_2 = 0;
            }
            if (cruise_acc_on == false && speed_below == true && (incoming.data.bytes[2] == 0x01 || incoming.data.bytes[2] == 0x04)) {
              id0x193.data.bytes[1] = cruise_min_speed;
              id0x193.data.bytes[3] = id0x193.data.bytes[3] | car_accelerator;
              id0x193.data.bytes[5] = acc_distance_set;
              loop_timer_id0x193 = micros();
              id0x193.data.bytes[6] = 0x01;
              cruise_acc_on = true;
              cruise_acc_on_low = true;
              speed_changed = true;
            }
            else {
              if (cruise_acc_on == true && speed_changed == false && (incoming.data.bytes[2] == 0x01 || incoming.data.bytes[2] == 0x04)) {
                if (incoming.data.bytes[2] == 0x01) {
                  id0x193.data.bytes[1] = id0x193.data.bytes[1] + 5;
                }
                if (incoming.data.bytes[2] == 0x04 && id0x193.data.bytes[1] >= cruise_min_speed + 5) {
                  id0x193.data.bytes[1] = id0x193.data.bytes[1] - 5;
                }
                loop_timer_id0x193 = micros();
                id0x193.data.bytes[6] = 0x01;
                speed_changed = true;
              }
              else {
                if (incoming.data.bytes[2] == 0x10 || incoming.data.bytes[2] == 0x90) {
                  cruise_acc_on_low = false;
                  cruise_acc_on = false;
                  cruise_cc_on = false;
                  cruise_cc_cut = false;
                }
              }
            }
            incoming.data.bytes[2] = 0x00;
            checksum = 0x01 + 0x94 + incoming.data.bytes[1] + incoming.data.bytes[2] + incoming.data.bytes[3];
            incoming.data.bytes[0] = checksum % 255;
            Can0.sendFrame(incoming);
          }
        }

//0x200
        else {
          if (incoming.data.bytes[1] >= 0xA0) {
            if (cruise_cc_on == false) {
              id0x193.data.bytes[1] = (incoming.data.bytes[0] / 10) + ((incoming.data.bytes[1] - 0xA0) * 25);
              id0x193.data.bytes[3] = id0x193.data.bytes[3] & 0xDF;
              id0x193.data.bytes[5] = acc_distance_set;
              loop_timer_id0x193 = micros();
              id0x193.data.bytes[6] = 0x01;
              cc_speed = id0x193.data.bytes[1];
              cruise_cc_on = true;
              cruise_acc_on = true;
            }
            else {
              cc_speed = (incoming.data.bytes[0] / 10) + ((incoming.data.bytes[1] - 0xA0) * 25);
            }
            id0x193.data.bytes[3] = id0x193.data.bytes[3] | car_accelerator;
          }
          else {
            if (cruise_acc_on == false && cruise_cc_on == false) {
              id0x193.data.bytes[1] = 0xFE;
              id0x193.data.bytes[3] = 0x01;
              id0x193.data.bytes[5] = 0x50;
            }
            id0x193.data.bytes[3] = id0x193.data.bytes[3] & 0xDF;
            cc_speed = 0x00;
          }
        }
    }

    if (Can0.available() > 0) {
      Can0.read(incoming);
      if (incoming.id != 0xBF) {
        if (incoming.id != 0x193) {

//0xD5
          if (incoming.id == 0xD5) {
            if (cruise_acc_on == true && cruise_cc_on == true && pedal_accerelator == false && (brake_auto_on == true || brake_cruise_on == true)) {
              id0xD5.data.bytes[1] = incoming.data.bytes[1];
              if (id0xD5_data_bytes4 == 0xFF) {
                id0xD5.data.bytes[4] = 0x7C;
                id0xD5.data.bytes[3] = 0xFF;
              }
              else {
                if (brake_force_control > brake_force_set + brake_force_control_base) {
                  brake_force_control = brake_force_control - brake_force_control_base;
                }
                id0xD5.data.bytes[4] = brake_force_control / 256;
                id0xD5.data.bytes[3] = brake_force_control - (id0xD5.data.bytes[4] * 256);
              }
              checksum = id0xD5.id + id0xD5.data.bytes[1] + id0xD5.data.bytes[2] + id0xD5.data.bytes[3] + id0xD5.data.bytes[4] + id0xD5.data.bytes[5];
              id0xD5.data.bytes[0] = checksum % 255;
              Can1.sendFrame(id0xD5);
              id0xD5_data_bytes4 = id0xD5.data.bytes[4];
              id0xD5_data_bytes3 = id0xD5.data.bytes[3];
            }
            else {
              if (brake_force_control > 31999 - 50) {
                Can1.sendFrame(incoming);
                id0xD5_data_bytes4 = incoming.data.bytes[4];
                id0xD5_data_bytes3 = incoming.data.bytes[3];
                id0xD5.data.bytes[3] = 0xFF;
                brake_force_control = 31999;
                brake_auto_on = false;
              }
              else {
                id0xD5.data.bytes[1] = incoming.data.bytes[1];
                brake_force_control = brake_force_control + 50;
                id0xD5.data.bytes[4] = brake_force_control / 256;
                id0xD5.data.bytes[3] = brake_force_control - (id0xD5.data.bytes[4] * 256);
                checksum = id0xD5.id + id0xD5.data.bytes[1] + id0xD5.data.bytes[2] + id0xD5.data.bytes[3] + id0xD5.data.bytes[4] + id0xD5.data.bytes[5];
                id0xD5.data.bytes[0] = checksum % 255;
                Can1.sendFrame(id0xD5);
                id0xD5_data_bytes4 = id0xD5.data.bytes[4];
                id0xD5_data_bytes3 = id0xD5.data.bytes[3];
              }
            }
          }

//0x159
          if (incoming.id == 0x159) {
            if (incoming.data.bytes[4] < 0xFF) {
              object_relativespeed_current = (incoming.data.bytes[4] & 0x0F) * 256 + incoming.data.bytes[3];
            }
            else {
              object_relativespeed_current = 0xFFF;
            }
          }

//0x15C
          if (incoming.id == 0x15C) {
            if (incoming.data.bytes[5] == 0x99 && incoming.data.bytes[4] <= 0x97 && incoming.data.bytes[4] > 0x92) {
              object_distance_current = (incoming.data.bytes[4] & 0x0F) * 256 + incoming.data.bytes[3] + car_speed_current;
            }
            else {
              if (incoming.data.bytes[5] > 0x99 && incoming.data.bytes[5] < 0x9E || (incoming.data.bytes[5] == 0x9E && object_relativespeed_current < 0xFFF)) {
                object_distance_current = 0xFFF + car_speed_current;
              }
            }
            if (incoming.data.bytes[5] == 0x9A || incoming.data.bytes[5] == 0x9B || incoming.data.bytes[5] == 0x9C || incoming.data.bytes[5] == 0x9D || incoming.data.bytes[7] == 0xF8 || incoming.data.bytes[7] == 0xFF) {
              object_distance_1 = false;
              object_distance_2 = false;
              object_distance_2_mid = false;
              object_distance_3 = false;
              object_distance_3_mid = false;
              object_distance_4 = false;
              object_distance_4_mid = false;
            }
            if (incoming.data.bytes[3] >= 0x66 && incoming.data.bytes[4] == 0x97 && incoming.data.bytes[7] == 0xFB)  {
              object_distance_1 = false;
              object_distance_2 = false;
              object_distance_2_mid = false;
              object_distance_3 = false;
              object_distance_3_mid = false;
              object_distance_4 = false;
              object_distance_4_mid = true;
            }
            if (incoming.data.bytes[3] < 0x66 && incoming.data.bytes[4] == 0x97 && incoming.data.bytes[7] == 0xFB)  {
              object_distance_1 = false;
              object_distance_2 = false;
              object_distance_2_mid = false;
              object_distance_3 = false;
              object_distance_3_mid = false;
              object_distance_4 = true;
              object_distance_4_mid = true;
            }
            if (incoming.data.bytes[3] >= 0x80 && incoming.data.bytes[4] == 0x96 && incoming.data.bytes[7] == 0xFB) {
              object_distance_1 = false;
              object_distance_2 = false;
              object_distance_2_mid = false;
              object_distance_3 = false;
              object_distance_3_mid = true;
              object_distance_4 = true;
              object_distance_4_mid = true;
            }
            if (incoming.data.bytes[3] < 0x80 && incoming.data.bytes[3] >= 0x00 && incoming.data.bytes[4] == 0x96 && incoming.data.bytes[7] == 0xFB) {
              object_distance_1 = false;
              object_distance_2 = false;
              object_distance_2_mid = false;
              object_distance_3 = true;
              object_distance_3_mid = true;
              object_distance_4 = true;
              object_distance_4_mid = true;
            }
            if (incoming.data.bytes[3] >= 0x80 && incoming.data.bytes[4] == 0x95 && incoming.data.bytes[7] == 0xFB) {
              object_distance_1 = false;
              object_distance_2 = false;
              object_distance_2_mid = true;
              object_distance_3 = true;
              object_distance_3_mid = true;
              object_distance_4 = true;
              object_distance_4_mid = true;
            }
            if (incoming.data.bytes[3] < 0x80 && incoming.data.bytes[3] >= 0x00 && incoming.data.bytes[4] == 0x95 && incoming.data.bytes[7] == 0xFB) {
              object_distance_1 = false;
              object_distance_2 = true;
              object_distance_2_mid = true;
              object_distance_3 = true;
              object_distance_3_mid = true;
              object_distance_4 = true;
              object_distance_4_mid = true;
            }
            if ((incoming.data.bytes[4] == 0x93 || incoming.data.bytes[4] == 0x94) && incoming.data.bytes[7] == 0xFB) {
              object_distance_1 = true;
              object_distance_2 = true;
              object_distance_2_mid = true;
              object_distance_3 = true;
              object_distance_3_mid = true;
              object_distance_4 = true;
              object_distance_4_mid = true;
            }
            if (cruise_acc_on == true && cruise_cc_on == true) {
              if ((incoming.data.bytes[6] == 0xA9 || brake_auto_on_latch == true || incoming.data.bytes[7] == 0xFC) && car_speed_current >= 0) {
                if (incoming.data.bytes[7] == 0xFC) {
                  brake_force_control_base = car_speed_current / 15 + 1;
                }
                else {
                  if (object_distance_current >= 0x630) {
                    brake_force_control_base = car_speed_current / 20 + (object_distance_current - 0x630) / 60 + 1;
                  }
                  else {
                    brake_force_control_base = car_speed_current / 20 + (0x630 - object_distance_current) / 60 + 1;
                  }
                }
                brake_auto_on = true;
                if (object_distance_current <= 0x680) {
                  brake_auto_on_latch = true;
                }
                else {
                  brake_auto_on_latch = false;
                }
              }
              else {
                brake_auto_on_latch = false;
                brake_auto_on = false;
              }
            if (cruise_acc_on == true && cruise_cc_on == true && (incoming.data.bytes[7] == 0xFB || incoming.data.bytes[7] == 0xFC)) {
              if (cc_speed > 0) {
                id0x193.data.bytes[3] = 0x31;
              }
              else {
                id0x193.data.bytes[3] = car_yellow;
              }
            }
            else {
              if (cc_speed > 0) {
                id0x193.data.bytes[3] = car_accelerator;
              }
              else {
                id0x193.data.bytes[3] = 0x01;
              }
            }
            if (brake_cruise_on == true || brake_auto_on == true) {
              id0x193.data.bytes[4] = car_red;
            }
            else {
              id0x193.data.bytes[4] = 0xF8;
            }
          }
        }
      }

//0x193
        else {
          id0x193.data.bytes[0] = incoming.data.bytes[0];
          Can1.sendFrame(id0x193);
        }
      }
    }
    
    if(micros() - loop_timer_id0x193 > 800000) id0x193.data.bytes[6] = 0x00;

//0x194
  if (counter_id0x194_1 <= 14) {
    if(micros() - loop_timer_id0x194 > 10000) {
      if (cruise_button_on == false && cruise_acc_on == true && cruise_cc_on == true) {
        if (object_distance_2_mid == true || object_distance_2 == true || object_distance_1 == true) {

          if (counter_id0x194_1 <= 3) {
            id0x194.data.bytes[2] = 0x10;
          }
          else {
            id0x194.data.bytes[2] = 0x00;
          }
          if (id0x194.data.bytes[1] == 0xFE) {
            id0x194.data.bytes[0] = id0x194.data.bytes[0] - 15;
            id0x194.data.bytes[1] = 0xF0;
          }
          else {
            id0x194.data.bytes[1]++;
          }
          id0x194.data.bytes[0] = id0x194.data.bytes[0] + 1 + (id0x194.data.bytes[2] - id0x194_data_bytes2);
          Can1.sendFrame(id0x194);
          id0x194_data_bytes2 = id0x194.data.bytes[2];
        }
        else {

          if (((id0x193.data.bytes[5] == 0x5D && object_distance_4_mid == false) || (id0x193.data.bytes[5] == 0x5C && object_distance_4 == false) || (id0x193.data.bytes[5] == 0x5B && object_distance_3_mid == false) || (id0x193.data.bytes[5] == 0x5A && object_distance_3 == false)) && id0x193.data.bytes[1] > cc_speed + 3) {
            if (counter_id0x194_2 == 5) {
              if (counter_id0x194_1 <= 3) {
                id0x194.data.bytes[2] = 0x01;
              }
              else {
                id0x194.data.bytes[2] = 0x00;
              }
              if (id0x194.data.bytes[1] == 0xFE) {
                id0x194.data.bytes[0] = id0x194.data.bytes[0] - 15;
                id0x194.data.bytes[1] = 0xF0;
              }
              else {
                id0x194.data.bytes[1]++;
              }
              id0x194.data.bytes[0] = id0x194.data.bytes[0] + 1 + (id0x194.data.bytes[2] - id0x194_data_bytes2);
              Can1.sendFrame(id0x194);
              id0x194_data_bytes2 = id0x194.data.bytes[2];
            }
          }

          if ((id0x193.data.bytes[5] == 0x5D && object_distance_4 == true) || (id0x193.data.bytes[5] == 0x5C && object_distance_3_mid == true) || (id0x193.data.bytes[5] == 0x5B && object_distance_3 == true) || id0x193.data.bytes[1] < cc_speed - 1) {
            if (counter_id0x194_2 == 5) {
              if (counter_id0x194_1 <= 3) {
                id0x194.data.bytes[2] = 0x04;
              }
              else {
                id0x194.data.bytes[2] = 0x00;
              }
              if (id0x194.data.bytes[1] == 0xFE) {
                id0x194.data.bytes[0] = id0x194.data.bytes[0] - 15;
                id0x194.data.bytes[1] = 0xF0;
              }
              else {
                id0x194.data.bytes[1]++;
              }
              id0x194.data.bytes[0] = id0x194.data.bytes[0] + 1 + (id0x194.data.bytes[2] - id0x194_data_bytes2);
              Can1.sendFrame(id0x194);
              id0x194_data_bytes2 = id0x194.data.bytes[2];
            }
          }
        }
      }
      loop_timer_id0x194 = micros();
      counter_id0x194_1++;
    }
  }

//0xAC LDM Error code D024
    if (counter_id0xAC <= 14) {
      if(micros() - loop_timer_id0xAC > 20000) {
          Can0.sendFrame(id0xAC);
          if (id0xAC.data.bytes[1] == 0xFE) {
            id0xAC.data.bytes[0] = id0xAC.data.bytes[0] - 15;
            id0xAC.data.bytes[1] = 0xF0;
          }
          else {
            id0xAC.data.bytes[1]++;
          }
        id0xAC.data.bytes[0]++;
        loop_timer_id0xAC = micros();
        counter_id0xAC++;
      }
    }
    else {
        counter_id0xAC = 0;
    }
  
//0xB4 LDM Error code D025
    if (counter_id0xB4 <= 14) {
      if(micros() - loop_timer_id0xB4 > 20000) {
          Can0.sendFrame(id0xB4);
          if (id0xB4.data.bytes[1] == 0x0E) {
            id0xB4.data.bytes[0] = id0xB4.data.bytes[0] - 15;
            id0xB4.data.bytes[1] = 0x00;
          }
          else {
            id0xB4.data.bytes[1]++;
          }
        id0xB4.data.bytes[0]++;
        loop_timer_id0xB4 = micros();
        counter_id0xB4++;
      }
    }
    else {
        counter_id0xB4 = 0;
    }

  }
}

// can_example application entry point
void loop()
{
}
