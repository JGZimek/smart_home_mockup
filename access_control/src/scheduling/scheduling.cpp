#include "scheduling.hpp"

#define SCHEDULING_TAG "app_scheduling"

TaskHandle_t wifiTaskHandle = NULL;
TaskHandle_t rfidTaskHandle = NULL;
TaskHandle_t pinpadTaskHandle = NULL;
TaskHandle_t mqttTaskHandle = NULL;
TaskHandle_t buttonTaskHandle = NULL;

Button button(BUTTON_PIN);
AccessPoint accessPoint("ESP_AP", "password123");

bool esp_setup()
{
    button.begin();

    // Sprawdzanie, czy dane WiFi już istnieją
    Preferences preferences;
    preferences.begin("wifi", true);                              // Tryb odczytu
    bool isConfigured = preferences.getBool("configured", false); // Flaga konfiguracji
    preferences.end();

    if (!isConfigured)
    {
        // Pierwsze uruchomienie – brak danych, uruchom tryb AP
        ESP_LOGI(SCHEDULING_TAG, "Device not configured. Starting in Access Point mode...");
        accessPoint.run(); // Czeka na zapisanie danych WiFi
    }

    while (true)
    {
        // Próba połączenia z WiFi
        ESP_LOGI(SCHEDULING_TAG, "Attempting to connect to WiFi...");
        if (init_wifi())
        {
            ESP_LOGI(SCHEDULING_TAG, "Connected to WiFi successfully!");

            // Inicjalizacja pozostałych modułów
            if (initialize_remaining_modules())
            {
                ESP_LOGI(SCHEDULING_TAG, "All modules initialized successfully!");
                break; // Program gotowy do działania
            }
            else
            {
                ESP_LOGE(SCHEDULING_TAG, "Failed to initialize some modules. Restarting Access Point...");
                accessPoint.run(); // Uruchom ponownie AP do poprawienia danych
            }
        }
        else
        {
            ESP_LOGW(SCHEDULING_TAG, "Failed to connect to WiFi. Restarting Access Point...");
            accessPoint.run(); // Uruchom ponownie AP do poprawienia danych
        }
    }

    return true;
}

bool initialize_remaining_modules()
{
    // Inicjalizacja pozostałych modułów
    if (!init_RFID())
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to initialize RFID");
        return false;
    }

    if (!init_pinpad())
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to initialize Pinpad");
        return false;
    }

    if (!init_scheduling())
    {
        ESP_LOGE(SCHEDULING_TAG, "Scheduling initialization failed. System halted.");
        return false;
    }

    return true;
}

void wifiTask(void *pvParameters)
{
    while (1)
    {
        handle_wifi();
        vTaskDelay(WIFI_RECONNECT_FREQ / portTICK_PERIOD_MS);
    }
}

void rfidTask(void *pvParameters)
{
    while (1)
    {
        handle_RFID();
        vTaskDelay(RFID_READ_FREQ / portTICK_PERIOD_MS);
    }
}

void pinpadTask(void *pvParameters)
{
    while (1)
    {
        handle_pinpad();
        vTaskDelay(PINPAD_READ_FREQ / portTICK_PERIOD_MS);
    }
}

void mqttTask(void *pvParameters)
{
    while (1)
    {
        handle_mqtt();
        vTaskDelay(MQTT_READ_FREQ / portTICK_PERIOD_MS);
    }
}

void buttonTask(void *pvParameters)
{
    while (1)
    {
        if (button.isLongPressed())
        {
            ESP_LOGI(SCHEDULING_TAG, "Button long press detected. Triggering AP mode...");
            accessPoint.run();
        }
        vTaskDelay(BUTTON_READ_FREQ / portTICK_PERIOD_MS);
    }
}

bool init_scheduling()
{
    ESP_LOGI(SCHEDULING_TAG, "Initializing scheduling...");

    BaseType_t result;

    result = xTaskCreatePinnedToCore(
        wifiTask,
        "Wifi Task",
        WIFI_TASK_STACK_SIZE,
        NULL,
        WIFI_TASK_PRIORITY,
        &wifiTaskHandle,
        WIFI_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create Network Task");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        rfidTask,
        "RFID Task",
        RFID_TASK_STACK_SIZE,
        NULL,
        RFID_TASK_PRIORITY,
        &rfidTaskHandle,
        RFID_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create RFID Task");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        pinpadTask,
        "Pinpad Task",
        PINPAD_TASK_STACK_SIZE,
        NULL,
        PINPAD_TASK_PRIORITY,
        &pinpadTaskHandle,
        PINPAD_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create Pinpad Task");
        return false;
    }

    // result = xTaskCreatePinnedToCore(
    //     mqttTask,
    //     "MQTT Task",
    //     MQTT_TASK_STACK_SIZE,
    //     NULL,
    //     MQTT_TASK_PRIORITY,
    //     &mqttTaskHandle,
    //     MQTT_CORE);

    // if (result != pdPASS)
    // {
    //     ESP_LOGE(SCHEDULING_TAG, "Failed to create MQTT Task");
    //     return false;
    // }

    result = xTaskCreatePinnedToCore(
        buttonTask,
        "Button Task",
        BUTTON_TASK_STACK_SIZE,
        NULL,
        BUTTON_TASK_PRIORITY,
        &buttonTaskHandle,
        BUTTON_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create Button Task");
        return false;
    }

    ESP_LOGI(SCHEDULING_TAG, "Scheduling initialized successfully.");
    return true;
}
