
#include <Arduino.h>

// WARNING WARNING WARNING
// THIS REQUIRES A SERIAL BUFFER OF SIZE 66 OR GREATER, MUST EDIT RINGBUFFER.H library!!!!
byte gps_msg[67];
int msg_lng = 66;


int getGPSdata(int *gps_fix, int *num_sats, double *lat, double *lon, double *gps_alt_m)
{
  int gps_data = 0; // = 0 = no data, 1 = data, 2 = checksum error
  int gps_string_valid = 1;

  while(Serial2.available() >= msg_lng)
  {
    if(gps_string_valid == 1)
    {
      gps_msg[0] = Serial2.read();
      gps_msg[1] = Serial2.read();
      gps_msg[2] = Serial2.read();
      gps_msg[3] = Serial2.read();
      gps_msg[4] = Serial2.read();
    }
    else
    {
      for(int i=0; i<4; i++)
      {
        gps_msg[i] = gps_msg[i+1];
      }
      gps_msg[4] = Serial2.read();
    }

    if(gps_msg[0]==0xA0 && gps_msg[1]==0xA1 && gps_msg[2]==0x00 && gps_msg[3]==0x3B && gps_msg[4]==0xA8)
    {
      // Data message
      for(int i=5;i<=msg_lng-1; i++)
      {
        gps_msg[i] = Serial2.read();
      }
      // Check checksum
      byte CS = 0x00;
      for(int i = 4;i<msg_lng-4;i++)
      {
        CS = CS^gps_msg[i];
      }
      byte checksum = gps_msg[msg_lng-3];
      if (CS == checksum)
      {
        // Data good, store into variables
        gps_data = 1;

        *gps_fix = (int)gps_msg[3+2];
        *num_sats= (int)gps_msg[3+3];
        *lat = (double)( (int)gps_msg[3+10] * 16777216 + (int)gps_msg[3+11]*65536 + (int)gps_msg[3+12]*256 + (int)gps_msg[3+13]) / 10000000;
        *lon = (double)( (int)gps_msg[3+14] * 16777216 + (int)gps_msg[3+15]*65536 + (int)gps_msg[3+16]*256 + (int)gps_msg[3+17]) / 10000000;
        *gps_alt_m = (double)( (int)gps_msg[3+22] * 16777216 + (int)gps_msg[3+23]*65536 + (int)gps_msg[3+24]*256 + (int)gps_msg[3+25]) / 100;
      }
      else
      {
        gps_data = 2;
      }
    }
    else{
      gps_string_valid = 0;
    }
  }
  
  return(gps_data);
}



void GPS2home(double lat,double lat_home,double lon,double lon_home,float psi, float *heading_home,float *dist_home_m)
{
  
  
}


