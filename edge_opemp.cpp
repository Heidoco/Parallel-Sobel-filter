#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <omp.h>

void sobel_filter(const cv::Mat& input, cv::Mat& output) {
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    #pragma omp parallel for
    for (int i = 1; i < input.rows - 1; i++) {
        for (int j = 1; j < input.cols - 1; j++) {
            int sx = 0, sy = 0;
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    sx += input.at<uchar>(i + x, j + y) * gx[x + 1][y + 1];
                    sy += input.at<uchar>(i + x, j + y) * gy[x + 1][y + 1];
                }
            }
            output.at<uchar>(i, j) = std::min(255, static_cast<int>(std::sqrt(sx * sx + sy * sy)));
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <numero_de_threads>" << std::endl;
        return -1;
    }

    int num_threads = std::atoi(argv[1]);
    if (num_threads <= 0) {
        std::cerr << "Número inválido de threads. Usando 1 thread por padrão." << std::endl;
        num_threads = 1;
    }

    // numero de threads do OpenMP
    omp_set_num_threads(num_threads);

    std::string image_path = argv[2];
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Erro ao carregar a imagem! Verifique o caminho: " << image_path << std::endl;
        return -1;
    }
    cv::Mat output = cv::Mat::zeros(image.size(), CV_8U);

    // Aplicar o filtro
    sobel_filter(image, output);

    // Salvar a imagem
    std::string output_path = "imagem_bordas.jpg";
    cv::imwrite(output_path, output);

    return 0;
}
