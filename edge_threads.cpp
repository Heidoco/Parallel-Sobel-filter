#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <thread>
#include <vector>

void sobel_filter_thread(const cv::Mat& input, cv::Mat& output, int start_row, int end_row) {
    int gx[3][3] = {{-1, 0, 1},{-2, 0, 2},{-1, 0, 1}};
    int gy[3][3] = {{ 1,  2,  1},{ 0,  0,  0},{-1, -2, -1}};

    for (int i = start_row; i < end_row; i++) {
        for (int j = 1; j < input.cols - 1; j++) {
            int sx = 0, sy = 0;

            // Aplicação do filtro de Sobel nas direções x e y
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

void sobel_filter(const cv::Mat& input, cv::Mat& output, int num_threads) {
    std::vector<std::thread> threads;
    int rows_per_thread = (input.rows - 2) / num_threads;
    int start_row = 1;

    for (int i = 0; i < num_threads; i++) {
        int end_row = start_row + rows_per_thread;
        if (i == num_threads - 1) {
            end_row = input.rows - 1; // processa até o final
        }
        threads.emplace_back(sobel_filter_thread, std::cref(input), std::ref(output), start_row, end_row);
        start_row = end_row;
    }

    for (auto& t : threads) {
        t.join();
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <numero_de_threads> <caminho_da_imagem>" << std::endl;
        return -1;
    }

    int num_threads = std::atoi(argv[1]);
    if (num_threads <= 0) {
        std::cerr << "Número inválido de threads. Usando 1 thread por padrão." << std::endl;
        num_threads = 1;
    }

    // Carregar a imagem
    std::string image_path = argv[2];
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Erro ao carregar a imagem! Verifique o caminho: " << image_path << std::endl;
        return -1;
    }

    cv::Mat output = cv::Mat::zeros(image.size(), CV_8U);

    // Aplicar o filtro
    sobel_filter(image, output, num_threads);

    // Salvar a imagem
    std::string output_path = "imagem_bordas.jpg";
    cv::imwrite(output_path, output);

    return 0;
}
