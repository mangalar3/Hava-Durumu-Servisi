#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5031// Sunucunun dinleyeceği port numarası
#define BUFFER_SIZE 10 // İstemciden gelen ve giden mesajlar için tampon boyutu

// Hava durumu verileri (örnek)
const char *get_weather(const char *city) {
    if (strstr(city, "Istanbul") != NULL) return "Istanbul: 10°C, Clear";
    if (strstr(city, "Ankara") != NULL) return "Ankara: 3°C, Cloudy";
    if (strstr(city, "Izmir") != NULL) return "Izmir: 20°C, Sunny";
    return "City not found.";
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Soket oluştur
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Adres ve port bağlama
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Dinleme moduna geç
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // 4. İstemci bağlantısını kabul et
    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) >= 0) {
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        printf("Received: %s\n", buffer);

        // Hava durumu bilgisi al ve gönder
        const char *response = get_weather(buffer);
        send(new_socket, response, strlen(response), 0);
        printf("Sent: %s\n", response);

	for (short i=0; i<7; i++){
		buffer[i] = '\0';
	}
        close(new_socket);
    }

    if (new_socket < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    return 0;
}
