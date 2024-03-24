import cv2
import time
import queue
import threading
import logging


class Sensor:
    def get(self):
        raise NotImplementedError("Method get() not implemented")


class SensorHandler:
    def __init__(self, sensor: Sensor):
        self.queue = queue.Queue()
        self._sensor = sensor
        self._running = False
        self._stopped = False
        self._main_thread = threading.Thread(target=self.get_data_from_sensor, daemon=True)
        self._main_thread.start()

    def __del__(self):
        if not self._stopped:
            self.stop()

    def stop(self):
        self._running = False
        self._stopped = True
        del self._sensor
        self._main_thread.join()

    def get_data_from_sensor(self):
        self._running = True
        while self._running:
            data = self._sensor.get()
            if self.queue.empty():
                self.queue.put(data)


class SensorX(Sensor):
    def __init__(self, delay:float):
        self._delay = delay
        self._data = 0

    def get(self) -> int:
        time.sleep(self._delay)
        self._data += 1
        return self._data


class SensorCam(Sensor):
    def __init__(self, cam_name: int, video_resolution : tuple[int, int]):
        self._cam_name = cam_name
        self._video_resolution = video_resolution
        self._video_io = cv2.VideoCapture(self._cam_name)
        if not self._video_io.read()[0]:
            self._video_io.release()
            logger.error(f"Camera with id {cam_name} does not exist")
            raise ValueError(f"Camera with id {cam_name} does not exist")

    def __del__(self):
        self._video_io.release()

    def get(self):
        tries = 0
        while tries < 3:
            res, frame = self._video_io.read()
            if not res:
                logger.warning(f"Camera with id {self._cam_name} is not active. Trying to reaccess")
            else:
                return cv2.resize(frame, self._video_resolution)
            tries += 1
        logger.error(f"Camera with id {self._cam_name} is not active. Trying to reaccess")
        raise ValueError(f"Camera with id {self._cam_name} is not active")



class WindowImage:
    def __init__(self, delay: int):
        self._delay = delay
        self.window_name = "window"

    def show(self, img):
        cv2.imshow(self.window_name, img)
        if cv2.waitKey(self._delay) == ord('q'):
            return True
        return False

    def __del__(self):
        cv2.destroyWindow(self.window_name)


def handle_sensors_data():
    wSize, hSize = 512, 512
    handlers = [
    SensorHandler(SensorX(0.01)),
    SensorHandler(SensorX(0.1)),
    SensorHandler(SensorX(1)),
    SensorHandler(SensorCam(0, (wSize, hSize))),
    ]
    window = WindowImage(1)
    data = [0 for i in range(len(handlers))]
    while True:
        for i, handler in enumerate(handlers):
            if not handler.queue.empty():
                data[i] = handler.queue.get()
        img_to_show = data[3]
        cv2.rectangle(img_to_show, (wSize - 200, hSize - 100), (wSize, hSize), (255,255,255), -1)
        for i in range(3):
            cv2.putText(img_to_show, f"Sensor{i}:{data[i]}", (wSize - 190, hSize - (70 - i * 25)), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,0,0), 2)
        stop_signal = window.show(data[3])
        if stop_signal:
            break
    for handler in handlers:
        handler.stop()

if __name__ == "__main__":
    logger = logging.getLogger(__name__)
    logging.basicConfig(filename='./log/logs.log', level=logging.ERROR)
    handle_sensors_data()