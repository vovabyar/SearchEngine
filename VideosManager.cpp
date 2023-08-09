#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <Trie/trie.hpp>

using json = nlohmann::json;

struct Videos {
    int id; 
    std::string title;
    std::string author;
    int duration;
    int views;

    // for serialization
    void to_json(json& j) const {
        j = json{{"id", id}, {"title", title}, {"author", author}, {"duration", duration}, {"views", views}};
    }

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
    Videos createVideo(const std::string& title, const std::string& author, int duration, int views) {
        Videos video;
        video.id = getNextUniqueId();
        video.title = title;
        video.author = author;
        video.duration = duration;
        video.views = views;
        return video;
    }

    void addVideo(const Videos& video) {
        videos.push_back(video);
    }

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
            outputFile << jsonData.dump(4); 
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
    return 0;
}