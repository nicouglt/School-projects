#include "Switcher.h"

const int A = 2;
const int B = 10;
const int C = 20;
// Constant declarations for different positions
//120//130//140

Switcher::Switcher(){
    M5.begin();             // M5Core module initialization and rx/tx initialization
    Wire.begin();
    delay(100);
    rx_channel_init();
    tx_channel_init();
}

void Switcher::SetPosition(char address){
  switch(address){
    case 'A':
      WritePosition(SWITCHER_ADDR, A);
      delay(500);
      WritePosition(SWITCHER_ADDR, 200);
      break;

    case 'B':
      WritePosition(SWITCHER_ADDR, B);
      delay(500);
      WritePosition(SWITCHER_ADDR, 200);
      break;

    case 'C':
      WritePosition(SWITCHER_ADDR, C);
      delay(500);
      WritePosition(SWITCHER_ADDR, 200);
      break;

    default :
      M5.Lcd.println("Adress ERROR");
      break;
  }
}

// Position transmission to the servo module
void Switcher::WritePosition(uint8_t addr, uint8_t angle) {
    Wire.beginTransmission(GOPLUS_ADDR);
    Wire.write(SWITCHER_ADDR | addr);
    Wire.write(angle);
    Wire.endTransmission();
}


// RX and TX communication intializion   --Code found in M5 documentation--
void Switcher::rx_channel_init() {
    rmt_config_t rmt_rx;
    rmt_rx.channel                       = RMT_CHANNEL_0;
    rmt_rx.gpio_num                      = GPIO_NUM_35;
    rmt_rx.clk_div                       = 80;
    rmt_rx.mem_block_num                 = 4;
    rmt_rx.rmt_mode                      = RMT_MODE_RX;
    rmt_rx.rx_config.filter_en           = false;
    rmt_rx.rx_config.filter_ticks_thresh = 0;
    rmt_rx.rx_config.idle_threshold      = 5000;

    rmt_config(&rmt_rx);
    rmt_driver_install(rmt_rx.channel, 1000, 0);
}

void Switcher::tx_channel_init() {
    rmt_config_t rmt_tx;
    rmt_tx.rmt_mode                       = RMT_MODE_TX;
    rmt_tx.channel                        = RMT_CHANNEL_4;
    rmt_tx.gpio_num                       = GPIO_NUM_5;
    rmt_tx.mem_block_num                  = 4;
    rmt_tx.clk_div                        = 80;
    rmt_tx.tx_config.loop_en              = false;
    rmt_tx.tx_config.carrier_duty_percent = 50;
    rmt_tx.tx_config.carrier_freq_hz      = 38000;
    rmt_tx.tx_config.carrier_level        = RMT_CARRIER_LEVEL_HIGH;
    rmt_tx.tx_config.carrier_en           = 1;
    rmt_tx.tx_config.idle_level           = RMT_IDLE_LEVEL_LOW;
    rmt_tx.tx_config.idle_output_en       = true;
    rmt_config(&rmt_tx);
    rmt_driver_install(rmt_tx.channel, 0, 0);
}
