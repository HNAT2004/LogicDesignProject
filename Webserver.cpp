#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

std::unordered_map<std::string, std::pair<std::string, std::string>> loadCSV(const std::string& filename) {
    std::unordered_map<std::string, std::pair<std::string, std::string>> data;
    std::ifstream infile(filename);
    std::string line;

    if (infile.is_open()) {
        while (getline(infile, line)) {
            size_t commaIndex1 = line.find(',');
            size_t commaIndex2 = line.find(',', commaIndex1 + 1);

            std::string csvUID = line.substr(0, commaIndex1);
            std::string csvHoTen = line.substr(commaIndex1 + 1, commaIndex2 - commaIndex1 - 1);
            std::string csvMaSoSinhVien = line.substr(commaIndex2 + 1);

            data[csvUID] = { csvHoTen ,csvMaSoSinhVien };
        }
        infile.close();
    }
    else {
        std::cerr << "Không thể mở file CSV." << std::endl;
    }

    return data;
}

void writeDataToCSV(const std::string& filename, const std::string& maSoSinhVien, const std::string& hoTen) {
    std::ofstream outfile;
    outfile.open(filename, std::ios_base::app | std::ios_base::binary); // Mở file ở chế độ append và binary

    // Lấy ngày và giờ hiện tại
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    char date[11];
    char time[9];
    std::strftime(date, sizeof(date), "%Y-%m-%d", now);
    std::strftime(time, sizeof(time), "%H:%M:%S", now);

    // Ghi dữ liệu vào file CSV với mã hóa UTF-8
    
    outfile << maSoSinhVien << "," << hoTen << "," << date << "," << time << "\n";
    outfile.close();
}

void handleClient(SOCKET clientSocket, const std::unordered_map<std::string, std::pair<std::string, std::string>>& data) {
    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, 1024, 0);
    std::string request(buffer);
    std::cout << "Client request: " << request << std::endl;

    // Lấy UID từ request
    size_t uidPos = request.find("uid=");
    if (uidPos == std::string::npos) {
        std::cerr << "Invalid request format." << std::endl;
        closesocket(clientSocket);
        return;
    }
    std::string uid = request.substr(uidPos + 4, request.find(' ', uidPos) - uidPos - 4);

    // Kiểm tra UID trong dữ liệu đã tải
    auto it = data.find(uid);
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

    if (it != data.end()) {
        response += it->second.second;
        writeDataToCSV("D:/LogicDesignProject/Danh_Sach_Diem_Danh_Sinh_Vien.csv", it->second.second, it->second.first);
    }
    else {
        response += "INVALID";
        }
    // In UID gửi về
    std::cout << "UID received: " << uid << std::endl;

    // Gửi response
    send(clientSocket, response.c_str(), response.length(), 0);
    closesocket(clientSocket);  
}

int main() {
    // Khởi tạo Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Tạo socket
    SOCKET server_fd, new_socket;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Gán socket với địa chỉ và port
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Listen
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Tải dữ liệu từ file CSV vào bộ nhớ
    auto data = loadCSV("D:/LogicDesignProject/Danh_Sach_Sinh_Vien.csv");

    // Chấp nhận kết nối và xử lý client
    sockaddr_in clientAddress;
    int clientAddressSize = sizeof(clientAddress);
    while ((new_socket = accept(server_fd, (struct sockaddr*)&clientAddress, &clientAddressSize)) != INVALID_SOCKET) {
        handleClient(new_socket, data);
    }

    // Đóng socket
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
