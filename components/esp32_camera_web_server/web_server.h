#pragma once

#ifdef USE_ESP32

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/preferences.h"
#include "esphome/components/esp32_camera/esp32_camera.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

struct httpd_req;

namespace esphome {
namespace esp32_camera_web_server {

enum Mode { Stream, Snapshot };

class WebServer : public Component {
 public:
  WebServer();
  ~WebServer();

  void setup() override;
  void on_shutdown() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void set_port(uint16_t port) { this->port_ = port; }
  void set_mode(Mode mode) { this->mode_ = mode; }
  void loop() override;

 protected:
  std::shared_ptr<esphome::esp32_camera::CameraImage> wait_for_image();
  esp_err_t handler_(struct httpd_req *req);
  esp_err_t streaming_handler_(struct httpd_req *req);
  esp_err_t snapshot_handler_(struct httpd_req *req);

 protected:
  uint16_t port_{0};
  void *httpd_{nullptr};
  SemaphoreHandle_t semaphore_;
  std::shared_ptr<esphome::esp32_camera::CameraImage> image_;
  bool running_{false};
  Mode mode_{Stream};
};

}  // namespace esp32_camera_web_server
}  // namespace esphome

#endif  // USE_ESP32
