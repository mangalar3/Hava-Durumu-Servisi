#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char city[BUFFER_SIZE];

    // 1. Soket oluştur
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 2. Sunucu adresine bağlan
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // 3. Kullanıcıdan şehir adı al ve sunucuya gönder
    printf("Enter city name: ");
    fgets(city, BUFFER_SIZE, stdin);
    city[strcspn(city, "\n")] = '\0'; // Yeni satırı kaldır
    send(sock, city, strlen(city), 0);

    // 4. Sunucudan yanıt al ve göster
    int valread = read(sock, buffer, BUFFER_SIZE);
    printf("Weather Info: %s\n", buffer);

    close(sock);
    return 0;
}
