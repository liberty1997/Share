#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "window.h"
#include <QFileDialog>
#include <clocale>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/highgui.hpp>

Window::Window(QWidget * parent) :
    QWidget(parent) {
    setupUi(this);
}

// Select Files
void Window::on_pushButton_clicked() {
    QFileDialog fileDialog;
    fileDialog.setWindowTitle("Select Files");
    fileDialog.setDirectory(".");
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setViewMode(QFileDialog::Detail);
    if(fileDialog.exec()) {
        fileNames = fileDialog.selectedFiles();
        QString f;
        for(int i = 0; i < fileNames.size(); ++i) {
            f.append(fileNames.at(i));
            f.append("\n");
        }
        textBrowser->setPlainText(f);
    }
}

/*
 * Deskewing
 *
 * Probabilistic Hough Transform for line detection
 *
 * compute the mean angle of all the lines
 */
void Window::on_pushButton_2_clicked() {
    if(fileNames.size() > 0) {
        cv::Mat img, binary;
        cv::Mat_<uchar>::iterator it;
        cv::Mat_<uchar>::iterator end;
        cv::Mat rot_mat;
        cv::Mat rotated;
        cv::Mat cropped;
        cv::Size box_size;
        std::string imgName, newname;
        size_t p;
        QStringList fileNames_new;
        for(int i = 0; i < fileNames.size(); ++i) {
            imgName = fileNames.at(i).toStdString();
            img = cv::imread(imgName, cv::IMREAD_GRAYSCALE);
            if( img.empty() ) { textBrowser->setPlainText("Error loading img."); }
            threshold(img, binary, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
            //bitwise_not(binary, binary);
            std::vector<cv::Point> points;
            //findNonZero(binary, points);
            it = binary.begin<uchar>();
            end = binary.end<uchar>();
            for(; it != end; ++it) {
                if(!*it) {
                    points.push_back(it.pos());
                }
            }
            cv::RotatedRect box = minAreaRect(points);
            double angle = static_cast<double>(box.angle);
            if(angle < -45.)
                angle += 90.;
            cv::Point2f vertices[4];
            box.points(vertices);
            for(int i = 0; i < 4; ++i)
                line(binary, vertices[i], vertices[(i + 1) % 4], cv::Scalar(105, 105, 105));
            // Rotation
            rot_mat = getRotationMatrix2D(box.center, angle, 1);
            warpAffine(binary, rotated, rot_mat, binary.size(), cv::INTER_CUBIC);
            // Cropping
            box_size = box.size;
            if(static_cast<double>(box.angle) < -45.)
                std::swap(box_size.width, box_size.height);
            getRectSubPix(rotated, box_size, box.center, cropped);
            p = imgName.find_last_of('.');
            newname = imgName.substr(0, p) + "_de" + imgName.substr(p, imgName.length() - p);
            imwrite(newname, cropped);
            fileNames_new.append(QString::fromStdString(newname));
        }
        fileNames = fileNames_new;
        QString f;
        for(int i = 0; i < fileNames.size(); ++i) {
            f.append(fileNames.at(i));
            f.append("\n");
        }
        textBrowser->setPlainText(f);
    }
    else {
        textBrowser->setPlainText("You have to select one file.");
    }
}

// Run Tesseract-OCR
void Window::on_pushButton_3_clicked() {
    if(fileNames.size() > 0) {
        setlocale(LC_ALL, "C"); // In Tesseract's baseapi.cpp, it requires "LC_ALL" to be "C" or "C.UTF-8".
                                // Mine is "en_US.UTF-8".
                                // https://github.com/tesseract-ocr/tesseract/blob/master/src/api/baseapi.cpp
        QString fileName;
        QString results;
        char * outText = nullptr;
        Pix * image = nullptr;
        tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
        // Initialize tesseract-ocr with English, without specifying tessdata path
        if(api->Init(nullptr, "eng")) {
            textBrowser->setPlainText("Could not initialize tesseract.\n");
        }
        for(int i = 0; i < fileNames.size(); ++i) {
            fileName = fileNames.at(i);
            results.append(fileName);
            results.append("\n");
            // Open input image with leptonica library
            image = pixRead(fileName.toStdString().c_str());
            api->SetImage(image);
            // Get OCR result
            outText = api->GetUTF8Text();
            results.append(outText);
            results.append("-------------------我是分割线-------------------\n");
        }
        // Destroy used object and release memory
        api->End();
        delete outText;
        pixDestroy(&image);
        results.append("\n");
        textBrowser->setPlainText(results);
    }
    else {
        textBrowser->setPlainText("You did not select any files.");
    }
}
