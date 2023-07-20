#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <Trie/trie.hpp>

using json = nlohmann::json;

// Определение структуры Videos
struct Videos {
    int id; // Добавляем поле id как индекс
    std::string title;
    std::string author;
    int duration;
    int views;

    // Метод для сериализации в JSON
    void to_json(json& j) const {
        j = json{{"id", id}, {"title", title}, {"author", author}, {"duration", duration}, {"views", views}};
    }

    // Метод для десериализации из JSON
    void from_json(const json& j) {
        if (j.find("id") != j.end()) {
            j.at("id").get_to(id);
        }
        j.at("title").get_to(title);
        j.at("author").get_to(author);
        j.at("duration").get_to(duration);
        j.at("views").get_to(views);
    }
};

class VideosManager {
public:
    // Создать структуру Videos и заполнить данными, присваивая уникальный id
    Videos createVideo(const std::string& title, const std::string& author, int duration, int views) {
        Videos video;
        video.id = getNextUniqueId(); // Получаем уникальный id
        video.title = title;
        video.author = author;
        video.duration = duration;
        video.views = views;
        return video;
    }

    // Добавить структуру Videos в коллекцию
    void addVideo(const Videos& video) {
        videos.push_back(video);
    }

    // Сохранить коллекцию Videos в файл JSON
    void saveToJson(const std::string& filename) {
        json jsonData;

        for (const Videos& video : videos) {
            json videoData;
            video.to_json(videoData);
            jsonData.push_back(videoData);
        }

        std::string filePath = getResourcePath() + filename;

        std::ofstream outputFile(filePath);
        if (outputFile.is_open()) {
            outputFile << jsonData.dump(4); // Записываем JSON с отступами (4 пробела)
            outputFile.close();
            std::cout << "Файл обновлен по пути: " << filePath << std::endl;
        } else {
            std::cerr << "Не удалось открыть файл для записи." << std::endl;
        }
    }

    void loadFromJson(const std::string& filename) {
        std::string filePath = getResourcePath() + filename;

        std::ifstream inputFile(filePath);
        if (inputFile.is_open()) {
            json loadedData;
            inputFile >> loadedData;

            videos.clear(); // Clear the existing videos before loading new ones

            for (const auto& entry : loadedData) {
                Videos loadedVideo;
                loadedVideo.from_json(entry);
                addVideo(loadedVideo);
            }

            inputFile.close();
            std::cout << "Файл загружен из пути: " << filePath << std::endl;
        } else {
            std::cerr << "Не удалось открыть файл для чтения." << std::endl;
        }
    }

    // Add a method to populate the Trie with video titles
    void populateTrie() {
        for (const Videos& video : videos) {
            // Insert video titles into the Trie
            titleTrie.insert(video.title);
            // Create a mapping between titles and video objects
            titleToVideo[video.title] = video;
        }
    }

    // Add a method to search for video titles using Trie and return similar titles in JSON format
    json searchSimilarTitles(const std::string& query) {
        json resultJson;

        // Use the Trie's complete function to get a vector of pointers to matching titles
        std::vector<const std::string*> results = titleTrie.complete(query);

        // Convert the pointers to std::string and store them in a new vector
        std::vector<std::string> similarTitles;
        for (const std::string* result : results) {
            similarTitles.emplace_back(*result);
        }

        // Add video objects to the JSON array
        for (const std::string& title : similarTitles) {
            if (titleToVideo.find(title) != titleToVideo.end()) {
                const Videos& video = titleToVideo[title];
                json videoData;
                videoData["id"] = video.id;
                videoData["title"] = video.title;
                videoData["author"] = video.author;
                videoData["duration"] = video.duration;
                videoData["views"] = video.views;
                resultJson.push_back(videoData);
            }
        }
        return resultJson;
    }

    const std::vector<Videos>& getVideos() const {
        return videos;
    }

private:
    std::string getResourcePath() const {
        return "/home/vovabyar/CLionProjects/trie/resources/";
    }

    int getNextUniqueId() {
        static int idCounter = 0; // Счетчик для уникальных id
        return idCounter++;
    }
    std::vector<Videos> videos;
    trie <std::string> titleTrie; // Trie to store video titles
    std::unordered_map<std::string, Videos> titleToVideo; // Mapping between titles and video objects

};

int main() {
    VideosManager manager;

//    // Создаем и заполняем структуры Videos
//    Videos video1 = manager.createVideo("Video 1", "Author 1", 120, 1000);
//    Videos video2 = manager.createVideo("Video 2", "Author 2", 20, 91000);
//
//    // Добавляем структуры Videos в коллекцию
//    manager.addVideo(video1);
//    manager.addVideo(video2);
//
//    // Сохраняем коллекцию Videos в файл JSON
//    manager.saveToJson("videos.json");

    // Загружаем коллекцию Videos из файла JSON
    manager.loadFromJson("videos.json");

    // Populate the Trie with video titles
    manager.populateTrie();

    std::string searchQuery = "The A";
    json searchResults = manager.searchSimilarTitles(searchQuery);

    std::cout << "Search results for '" << searchQuery << "':" << std::endl;
    std::cout << searchResults.dump(4) << std::endl; // Print the JSON results

    std::string searchResultsFilename = "/home/vovabyar/CLionProjects/trie/resources/search_results.json";
    std::ofstream searchResultsFile(searchResultsFilename);
    if (searchResultsFile.is_open()) {
        searchResultsFile << searchResults.dump(4);
        searchResultsFile.close();
        std::cout << "Результаты поиска сохранены в файл: " << searchResultsFilename << std::endl;
    } else {
        std::cerr << "Не удалось открыть файл для записи результатов поиска." << std::endl;
    }
    // Выводим загруженные видео
//    std::cout << "Загруженные видео:" << std::endl;
//    const std::vector<Videos>& loadedVideos = manager.getVideos();
//    for (const Videos& loadedVideo : loadedVideos) {
//        std::cout << "ID: " << loadedVideo.id << std::endl;
//        std::cout << "Название: " << loadedVideo.title << std::endl;
//        std::cout << "Автор: " << loadedVideo.author << std::endl;
//        std::cout << "Продолжительность: " << loadedVideo.duration << " сек" << std::endl;
//        std::cout << "Просмотры: " << loadedVideo.views << std::endl;
//        std::cout << std::endl;
//    }

    return 0;
}


/*
 * Генерация входных данных.
 * */
//int main() {
//    std::ifstream moviesFile("/home/vovabyar/CLionProjects/trie/resources/movies.txt");
//    if (!moviesFile.is_open()) {
//        std::cerr << "Не удалось открыть файл 'movies.txt'." << std::endl;
//        return 1;
//    }
//    VideosManager manager;
//
//    std::vector<std::string> videoTitles;
//    std::string line;
//    while (std::getline(moviesFile, line)) {
//        videoTitles.push_back(line);
//    }
//    moviesFile.close();
//
//    json jsonData;
//
//    for (int id = 0; id < videoTitles.size(); ++id) {
//         Videos video = manager.createVideo(videoTitles[id], "Author " + std::to_string(id + 1), rand() % 300 + 60, rand() % 10000000);
//         manager.addVideo(video);
//
//    }
//
//    manager.saveToJson("videos.json");
//
//    return 0;
//}