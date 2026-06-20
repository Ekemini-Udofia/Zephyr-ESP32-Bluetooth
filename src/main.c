#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

// #include <zephyr/bluetooth/gap.h>

// Generate a Custom 128 bit UUID for our bluetooth service
// YOu can generate random UUIDs online. This one is unique to our test
#define TEST_SERVICE_UUID BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x567812345678)

// Build our static GATT Database
BT_GATT_SERVICE_DEFINE(esp32_test_service,
  BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(TEST_SERVICE_UUID)),
  BT_GATT_CHARACTERISTIC(BT_UUID_GAP_DEVICE_NAME, BT_GATT_CHRC_READ,
                         BT_GATT_PERM_READ, NULL, NULL, NULL)
);

static const struct bt_data ad[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
  BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, (sizeof(CONFIG_BT_DEVICE_NAME) - 1)),
};

static void bt_ready(int err) {
  if (err) {
    printk("Bluetooth initialization failed (err %d)\n", err);
    return;
  }

  printk("Bluetooth initialized Successfully!\n");

  // TUrn on the radio and starat blinking the advertisement packets
  err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);

  if (err) {
    printk("Advertising failed to start (err %d)\n", err);
    return;
  }

  printk("Advertisement started! Look for  '%s' on your phone.\n", CONFIG_BT_DEVICE_NAME);
}

int main(void) {
  int err;

  printk("\n--Booting Classic ESP32 Bluetooth Beacon--\n\n");

  // Initialize core BLuetooth Host Subsystem stack
  err = bt_enable(bt_ready);

  if (err) {
    printk("Bluetooth enablement crashed immediately (err %d)\n", err);
    return 0;
  }

  printk("Looping.");

  while (true) {
    printk(".");
    k_msleep(2000);
  }

  return 0;
}