#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

int main() {
    cv::VideoCapture cam(0);
    if (!cam.isOpened()) {
        std::cerr << "Ошибка: не удалось открыть веб-камеру!" << std::endl;
        return -1;
    }

    cv::CascadeClassifier faceCascade;
    std::string cascadePath = "/home/vladimir/Study/OpenCV/College_code/KT3/OpenCV_KT3/haarcascade_frontalface_default.xml";

    if (!faceCascade.load(cascadePath)) {
        std::cerr << "Ошибка: не удалось загрузить файл каскада: " << cascadePath << std::endl;
        return -1;
    }

    std::string dirName = "/home/vladimir/Study/OpenCV/College_code/KT3/video_faces";
    std::filesystem::create_directory(dirName);

    std::cout << "Программа запущена.\n"
              << "Нажмите 'r' или 'к' для включения/выключения сохранения лиц.\n"
              << "Нажмите 'ESC' для выхода." << std::endl;

    cv::Mat frame, gray;
    bool isSaving = false;
    int savedCount = 0;

    while (true) {
        cam >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(gray, faces, 1.1, 4, 0, cv::Size(30, 30));

        for (size_t i = 0; i < faces.size(); i++) {
            cv::rectangle(frame, faces[i], cv::Scalar(0, 255, 0), 2);

            if (isSaving) {
                cv::Rect faceRect = faces[i];

                if (faceRect.x >= 0 && faceRect.y >= 0 &&
                    faceRect.x + faceRect.width <= frame.cols &&
                    faceRect.y + faceRect.height <= frame.rows) {

                    cv::Mat faceRoi = frame(faceRect);
                    std::string filename = dirName + "/face_" + std::to_string(savedCount++) + ".jpg";
                    cv::imwrite(filename, faceRoi);
                }
            }
        }

        if (isSaving) {
            cv::putText(frame, "RECORDING...", cv::Point(30, 50),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
        }

        cv::imshow("Face Saver", frame);

        int key = cv::waitKey(30);
        if (key == 27) {
            break;
        } else if (key == 'r' || key == 'R' || key == 'к' || key == 'К') {
            isSaving = !isSaving;
            std::cout << "Сохранение лиц: " << (isSaving ? "ВКЛ" : "ВЫКЛ") << std::endl;
        }
    }

    cv::destroyAllWindows();
    return 0;
}