#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <bitset>
#include <unordered_set>
#include <cmath> // для використання функції ln

class BloomFilter {
private:
    std::vector<std::function<unsigned int(unsigned char*, int, unsigned int, unsigned int)>> hashFuncs;
    std::bitset<65536> bitArray; // Бітовий масив розміром 2^16 біт

public:
    BloomFilter(std::vector<std::function<unsigned int(unsigned char*, int, unsigned int, unsigned int)>> funcs)
        : hashFuncs(std::move(funcs)) {}

    void add(unsigned char* element, int length, unsigned int k, unsigned int N) {
        if (length <= 100) {
            for (const auto& hashFunc : hashFuncs) {
                size_t hash = hashFunc(element, length, k, N);
                bitArray.set(hash, true);
            }
        }
        else {
            std::cout << "Element length exceeds the limit of 100 characters." << std::endl;
        }
    }

    bool contains(unsigned char* element, int length, unsigned int k, unsigned int N) {
        if (length <= 100) {
            for (const auto& hashFunc : hashFuncs) {
                size_t hash = hashFunc(element, length, k, N);
                if (!bitArray.test(hash)) {
                    return false;
                }
            }
            return true;
        }
        else {
            std::cout << "Element length exceeds the limit of 100 characters." << std::endl;
            return false;
        }
    }

    void clear() {
        bitArray.reset();
    }
};

std::string generateRandomString() {
    std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> length_dist(1, 100);
    int length = length_dist(gen);
    std::string result;
    for (int i = 0; i < length; ++i) {
        result += characters[std::uniform_int_distribution<>(0, characters.size() - 1)(gen)];
    }
    return result;
}

unsigned int polynomial_hash(unsigned char* string, int length, unsigned int k, unsigned int N) {
    unsigned int hash_value = 0;
    unsigned int Y = 1;

    for (int i = length - 1; i >= 0; i -= 2) {
        unsigned int current_value = string[i];
        if (i > 0) {
            current_value += string[i - 1] << 8;
        }

        hash_value = (hash_value * k + current_value) % N;
        Y = (Y * k) % N;
    }

    hash_value = (hash_value + Y) % N;
    return hash_value;
}

unsigned int calculateOptimalHashFunctions(int n, int N) {
    double a = static_cast<double>(n) / static_cast<double>(N);
    double s = (1.0 / a) * std::log(2.0);
    return static_cast<unsigned int>(std::round(s));
}

unsigned int generateUniqueK(std::unordered_set<unsigned int>& used_keys) {
    std::random_device rd;
    std::mt19937 gen(rd());
    unsigned int new_key;
    do {
        new_key = std::uniform_int_distribution<>(0, 65535)(gen);
    } while (used_keys.count(new_key) > 0);
    used_keys.insert(new_key);
    return new_key;
}

int main() {
    
    //Геш-функція
    /*
    std::string input_string = generateRandomString(); // Генерувати випадковий рядок
    unsigned int N = 1 << 16; // За фіксованим значенням N = 2^16
    unsigned int n = input_string.length();
    unsigned int k = calculateOptimalHashFunctions(n, N); // Обчислити оптимальне k

    std::cout << "Generated string: " << input_string << std::endl;
    std::cout << "Generated k: " << k << std::endl;

    if (input_string.length() % 2 != 0) {
        input_string += '\0';
    }

    int length = input_string.length();
    int max_length = 100;
    if (length > max_length) {
        length = max_length;
        input_string = input_string.substr(0, max_length);
    }

    unsigned char* byte_array = new unsigned char[n];
    for (unsigned int i = 0; i < n; ++i) {
        byte_array[i] = static_cast<unsigned char>(input_string[i]);
    }

    unsigned int result = polynomial_hash(byte_array, n, k, N);
    std::cout << "Hash for '" << input_string << "' with coefficient k=" << k << " and module N=" << N << ": " << result << std::endl;

    delete[] byte_array;
    */
    
    //Фільтр Блума
    /*
    std::vector<std::function<unsigned int(unsigned char*, int, unsigned int, unsigned int)>> hashFunctions;
    hashFunctions.push_back(polynomial_hash);
    int numberOfElements = 100; // Задана кількість доданих елементів
    int sizeOfArray = 65536; // Розмір бітового масиву N

    unsigned int numOfHashFunctions = calculateOptimalHashFunctions(numberOfElements, sizeOfArray);
    std::cout << "Optimal number of hash functions: " << numOfHashFunctions << std::endl;

    BloomFilter filter(hashFunctions);
    std::unordered_set<unsigned int> usedKeys;

    std::string randomStringToAdd = generateRandomString();
    std::string randomStringToSearch = generateRandomString();

    std::cout << "Random string to add: " << randomStringToAdd << std::endl;

    unsigned int k = generateUniqueK(usedKeys);
    filter.add(reinterpret_cast<unsigned char*>(&randomStringToAdd[0]), randomStringToAdd.length(), k, sizeOfArray);
    std::cout << "Added: " << randomStringToAdd << std::endl;

    k = generateUniqueK(usedKeys);
    bool found = filter.contains(reinterpret_cast<unsigned char*>(&randomStringToSearch[0]), randomStringToSearch.length(), k, sizeOfArray);

    std::cout << "Random string to search: " << randomStringToSearch << std::endl;

    if (found) {
        std::cout << "The filter possibly contains: " << randomStringToSearch << std::endl;
    }
    else {
        std::cout << "The filter definitely does not contain: " << randomStringToSearch << std::endl;
    }

    filter.clear();
    */

    //експеримент
    /*
    double a = 0.1; 
    unsigned int N = 10000; 
    unsigned int k = calculateOptimalHashFunctions(a * N, N); 
    
    std::vector<std::function<unsigned int(unsigned char*, int, unsigned int, unsigned int)>> hashFunctions;
    unsigned int polynomial_hash(unsigned char* string, int length, unsigned int k, unsigned int N);
    hashFunctions.push_back(polynomial_hash);
    BloomFilter filter(hashFunctions);
    
    int experiments = 100;
    double total_error = 0.0;

    for (int exp = 0; exp < experiments; ++exp) {
    
        int n = a * N;
        std::unordered_set<std::string> addedMessages;

        for (int i = 0; i < n; ++i) {
            std::string message = generateRandomString(); // Генерація випадкового повідомлення
            addedMessages.insert(message); // Запам'ятовуємо повідомлення для порівняння
            filter.add(reinterpret_cast<unsigned char*>(&message[0]), message.length(), k, N); // Додавання до фільтру
        }

        // Перевірка наявності випадкових повідомлень до першого хибного спрацювання
        int M = 0;
        int false_positives = 0;

        while (true) {
            std::string message = generateRandomString(); // Генерація випадкового повідомлення
            ++M;

            if (addedMessages.find(message) == addedMessages.end() && filter.contains(reinterpret_cast<unsigned char*>(&message[0]), message.length(), k, N)) {
                ++false_positives; // Знайдено помилковий позитив
            }
            else {
                break; // Досягнуто першого хибного спрацювання
            }
        }

        double error_probability = false_positives / static_cast<double>(M); // Обчислення ймовірності помилки
        total_error += error_probability;
        filter.clear(); // Очищення фільтру для наступного експерименту
    }

    double average_error = total_error / experiments; // Середня ймовірність помилки за усіма експериментами
    std::cout << "Average Error Probability: " << average_error << std::endl;
    */

    //таблиця
    /*
    std::cout << "a\t\ts\t\tError Probability (p)" << std::endl;

    for (double a = 0.05; a <= 0.5; a += 0.05) {
        double s = (1.0 / a) * std::log(2.0); // Обчислення s відповідно до умови
        double N = 10000; // Обране значення N (розмір фільтру)
        unsigned int k = calculateOptimalHashFunctions(a * N, N); // Оптимальна кількість хеш-функцій

        std::vector<std::function<unsigned int(unsigned char*, int, unsigned int, unsigned int)>> hashFunctions;
        hashFunctions.push_back(polynomial_hash); // Додайте вашу хеш-функцію сюди
        BloomFilter filter(hashFunctions);

        int experiments = 100;
        double total_error = 0.0;

        for (int exp = 0; exp < experiments; ++exp) {
            int n = static_cast<int>(a * N); // Кількість повідомлень для додавання до фільтру
            std::unordered_set<std::string> addedMessages;

            // Додавання n випадкових повідомлень до фільтру
            for (int i = 0; i < n; ++i) {
                std::string message = generateRandomString();
                addedMessages.insert(message);
                filter.add(reinterpret_cast<unsigned char*>(&message[0]), message.length(), k, N);
            }

            // Перевірка наявності випадкових повідомлень до першого хибного спрацювання
            int M = 0;
            int false_positives = 0;

            while (true) {
                std::string message = generateRandomString();
                ++M;

                if (addedMessages.find(message) == addedMessages.end() && filter.contains(reinterpret_cast<unsigned char*>(&message[0]), message.length(), k, N)) {
                    ++false_positives; // Знайдено помилковий позитив
                }
                else {
                    break; // Досягнуто першого хибного спрацювання
                }
            }

            double error_probability = false_positives / static_cast<double>(M);
            total_error += error_probability;
            filter.clear(); // Очищення фільтру для наступного експерименту
        }

        double average_error = total_error / experiments;
        std::cout << a << "\t\t" << s << "\t\t" << average_error << std::endl;
    }
    */

    return 0;
}
