
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/lwip_napt.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include <string.h>

#include "connect.h"
#include "main.h"

#if CONFIG_ESP_WPA3_SAE_PWE_HUNT_AND_PECK
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HUNT_AND_PECK
#define EXAMPLE_H2E_IDENTIFIER ""

#elif CONFIG_ESP_WPA3_SAE_PWE_HASH_TO_ELEMENT
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HASH_TO_ELEMENT
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID

#elif CONFIG_ESP_WPA3_SAE_PWE_BOTH
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_BOTH
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#endif

#if CONFIG_ESP_WIFI_AUTH_OPEN
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_OPEN
#elif CONFIG_ESP_WIFI_AUTH_WEP
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WEP
#elif CONFIG_ESP_WIFI_AUTH_WPA_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WAPI_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WAPI_PSK
#endif

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

static const char * TAG = "wifi station";

static int s_retry_num = 0;

static void event_handler(void * arg, esp_event_base_t event_base, int32_t event_id, void * event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {

        // Wait a little
        vTaskDelay(2500 / portTICK_PERIOD_MS);
        esp_wifi_connect();
        s_retry_num++;
        ESP_LOGI(TAG, "Retrying WiFi connection...");
        MINER_set_wifi_status(WIFI_RETRYING, s_retry_num);

    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t * event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(TAG, "Bitaxe ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void generate_ssid(char * ssid)
{
    uint8_t mac[6];
    esp_wifi_get_mac(ESP_IF_WIFI_AP, mac);
    // Format the last 4 bytes of the MAC address as a hexadecimal string
    snprintf(ssid, 32, "Bitaxe_%02X%02X", mac[4], mac[5]);
}

esp_netif_t * wifi_init_softap(void)
{
    esp_netif_t * esp_netif_ap = esp_netif_create_default_wifi_ap();

    // Define a buffer for the SSID
    char ssid_with_mac[13]; // "Bitaxe" + 4 bytes from MAC address

    // Generate the SSID
    generate_ssid(ssid_with_mac);

    wifi_config_t wifi_ap_config;
    memset(&wifi_ap_config, 0, sizeof(wifi_ap_config)); // Clear the structure
    strncpy((char *) wifi_ap_config.ap.ssid, ssid_with_mac, sizeof(wifi_ap_config.ap.ssid));
    wifi_ap_config.ap.ssid_len = strlen(ssid_with_mac);
    wifi_ap_config.ap.channel = 1;
    wifi_ap_config.ap.max_connection = 30;
    wifi_ap_config.ap.authmode = WIFI_AUTH_OPEN;
    wifi_ap_config.ap.pmf_cfg.required = false;

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_config));

    return esp_netif_ap;
}

void toggle_wifi_softap(void)
{
    wifi_mode_t mode = WIFI_MODE_NULL;
    ESP_ERROR_CHECK(esp_wifi_get_mode(&mode));

    if (mode == WIFI_MODE_APSTA) {
        ESP_LOGI(TAG, "ESP_WIFI Access Point Off");
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    } else {
        ESP_LOGI(TAG, "ESP_WIFI Access Point On");
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    }
}

void wifi_softap_off(void)
{
    ESP_LOGI(TAG, "ESP_WIFI Access Point Off");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
}

void wifi_softap_on(void)
{
    ESP_LOGI(TAG, "ESP_WIFI Access Point On");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
}

/* Initialize wifi station */
esp_netif_t * wifi_init_sta(const char * wifi_ssid, const char * wifi_pass)
{
    esp_netif_t * esp_netif_sta = esp_netif_create_default_wifi_sta();

    wifi_config_t wifi_sta_config = {
        .sta =
            {
                /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
                 * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
                 * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
                 * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
                 */
                .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
                // .sae_pwe_h2e = ESP_WIFI_SAE_MODE,
                // .sae_h2e_identifier = EXAMPLE_H2E_IDENTIFIER,
            },
    };
    strncpy((char *) wifi_sta_config.sta.ssid,
            wifi_ssid,
            sizeof(wifi_sta_config.sta.ssid));
    wifi_sta_config.sta.ssid[sizeof(wifi_sta_config.sta.ssid) - 1] = '\0';
    strncpy((char *) wifi_sta_config.sta.password, wifi_pass, 63);
    wifi_sta_config.sta.password[63] = '\0';

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_sta_config));

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    return esp_netif_sta;
}

void wifi_init(const char * wifi_ssid, const char * wifi_pass, const char * hostname)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

    /*Initialize WiFi */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    /* Initialize AP */
    ESP_LOGI(TAG, "ESP_WIFI Access Point On");
    esp_netif_t * esp_netif_ap = wifi_init_softap();

    /* Initialize STA */
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    esp_netif_t * esp_netif_sta = wifi_init_sta(wifi_ssid, wifi_pass);

    /* Start WiFi */
    ESP_ERROR_CHECK(esp_wifi_start());

    /* Set Hostname */
    esp_err_t err = esp_netif_set_hostname(esp_netif_sta, hostname);
    if (err != ERR_OK) {
        ESP_LOGW(TAG, "esp_netif_set_hostname failed: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "ESP_WIFI setting hostname to: %s", hostname);
    }

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    return;
}

EventBits_t wifi_connect(void)
{
    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */

    return bits;
}
