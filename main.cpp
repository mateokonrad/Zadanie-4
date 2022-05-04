// Mateusz Wabinski_2_rok_4_semestr_nr_indeksu_146326_Grupa_D1

#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;

uint8_t bit_error(uint8_t c1, uint8_t c2) {
    static const uint8_t bits[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
    uint8_t n = c1 ^ c2;
    return bits[n >> 4] + bits[0x0F & n];
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        cerr << "expected two arguments: [FILE 1] [FILE 2]\n";
        return 1;
    }
    

    ifstream file1(argv[1]);
    if (file1.is_open() == 0) {
        cerr << "file " << argv[1] << " cannot be opened\n";
        return 2;
    }

    ifstream file2(argv[2]);
    if (file2.is_open() == 0) {
        cerr << "file " << argv[2] << " cannot be opened\n";
        file1.close();
        return 2;
    }

    auto start = std::chrono::high_resolution_clock::now();

    long long total_error = 0;
    uint8_t c1, c2;
    while (file1 >> c1 && file2 >> c2) {
        total_error += bit_error(c1, c2);
    }
    

    file1.clear();
    file2.clear();
    file1.seekg(0, ios::end);
    file2.seekg(0, ios::end);

    long long size1 = file1.tellg();
    long long size2 = file2.tellg();
    long long total_size = max(size1, size2);

    file1.close();
    file2.close();

    total_error += 8 * abs(size2 - size1);
    float BER = float(total_error) / float(total_size * 8);

    auto end = std::chrono::high_resolution_clock::now();
    auto total_time = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "     files: " << argv[1] << " " << argv[2] << endl;
    cout << "  compared: " << 8 * total_size << endl;
    cout << " different: " << total_error << endl;
    cout << "       BER: " << BER << endl;
    cout << "      time: " << total_time.count() << " us" << endl;

    ofstream log_file("log.txt", ios::app);
    if (log_file.is_open() == 0) {
        cerr << "cannot open log file\n";
        return 3;
    }
    

    std::time_t current_time = std::chrono::system_clock::to_time_t(end);
    log_file << std::ctime(&current_time);
    log_file << "     files: " << argv[1] << " " << argv[2] << endl;
    log_file << "  compared: " << 8 * total_size << endl;
    log_file << " different: " << total_error << endl;
    log_file << "       BER: " << BER << endl;
    log_file << "      time: " << total_time.count() << " us" << endl << endl;

    log_file.close();

    return 0;
}
