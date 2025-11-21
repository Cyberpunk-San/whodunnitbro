#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>
#include <cctype>
#include <regex>
#include <cmath>      // ADD THIS LINE
#include <functional> 
namespace DetectiveUtils {

    // ==================== STRING UTILITIES ====================
    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r\f\v");
        if (start == std::string::npos) return "";
        
        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(start, end - start + 1);
    }

    std::string capitalizeWords(const std::string& str) {
        std::string result = str;
        bool capitalizeNext = true;
        
        for (char& c : result) {
            if (std::isspace(c)) {
                capitalizeNext = true;
            } else if (capitalizeNext) {
                c = std::toupper(c);
                capitalizeNext = false;
            } else {
                c = std::tolower(c);
            }
        }
        return result;
    }

    bool containsIgnoreCase(const std::string& str, const std::string& substr) {
        auto it = std::search(
            str.begin(), str.end(),
            substr.begin(), substr.end(),
            [](char ch1, char ch2) { 
                return std::toupper(ch1) == std::toupper(ch2); 
            }
        );
        return it != str.end();
    }

    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        
        while (std::getline(ss, token, delimiter)) {
            if (!token.empty()) {
                tokens.push_back(trim(token));
            }
        }
        return tokens;
    }

    std::string join(const std::vector<std::string>& strings, const std::string& delimiter) {
        std::ostringstream oss;
        for (size_t i = 0; i < strings.size(); ++i) {
            if (i > 0) oss << delimiter;
            oss << strings[i];
        }
        return oss.str();
    }

    bool startsWith(const std::string& str, const std::string& prefix) {
        return str.size() >= prefix.size() && 
               str.compare(0, prefix.size(), prefix) == 0;
    }

    bool endsWith(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() && 
               str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    std::string replaceAll(const std::string& str, const std::string& from, const std::string& to) {
        std::string result = str;
        size_t start_pos = 0;
        while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
            result.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return result;
    }

    // ==================== TIME & DATE UTILITIES ====================
    std::string getCurrentDateTime() {
        auto now = std::chrono::system_clock::now();
        return formatTimePoint(now);
    }

    std::string formatTimePoint(const std::chrono::system_clock::time_point& tp) {
        auto time = std::chrono::system_clock::to_time_t(tp);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    std::chrono::system_clock::time_point stringToTimePoint(const std::string& timeStr) {
        std::tm tm = {};
        std::stringstream ss(timeStr);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    int daysBetween(const std::chrono::system_clock::time_point& from, 
                   const std::chrono::system_clock::time_point& to) {
        auto duration = to - from;
        return std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24;
    }

    bool isRecent(const std::chrono::system_clock::time_point& timePoint, int daysThreshold) {
        auto now = std::chrono::system_clock::now();
        return daysBetween(timePoint, now) <= daysThreshold;
    }

    // ==================== VALIDATION UTILITIES ====================
    bool isValidName(const std::string& name) {
        if (name.empty() || name.length() > 50) return false;
        
        // Name should contain only letters, spaces, hyphens, and apostrophes
        for (char c : name) {
            if (!std::isalpha(c) && c != ' ' && c != '-' && c != '\'') {
                return false;
            }
        }
        return true;
    }

    bool isValidEmail(const std::string& email) {
        static const std::regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return std::regex_match(email, pattern);
    }

    bool isValidPhoneNumber(const std::string& phone) {
        static const std::regex pattern(R"(^[\+]?[0-9\s\-\(\)]{10,}$)");
        return std::regex_match(phone, pattern);
    }

    bool isValidDate(const std::string& date) {
        static const std::regex pattern(R"(^\d{4}-\d{2}-\d{2}$)");
        if (!std::regex_match(date, pattern)) return false;
        
        // Basic date validation
        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));
        
        return year >= 1900 && year <= 2100 && month >= 1 && month <= 12 && day >= 1 && day <= 31;
    }

    bool isStrongPassword(const std::string& password) {
        if (password.length() < 8) return false;
        
        bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
        for (char c : password) {
            if (std::isupper(c)) hasUpper = true;
            else if (std::islower(c)) hasLower = true;
            else if (std::isdigit(c)) hasDigit = true;
            else hasSpecial = true;
        }
        
        return hasUpper && hasLower && hasDigit;
    }

    bool isNumeric(const std::string& str) {
        if (str.empty()) return false;
        for (char c : str) {
            if (!std::isdigit(c) && c != '.' && c != '-') return false;
        }
        return true;
    }

    bool isAlphaNumeric(const std::string& str) {
        for (char c : str) {
            if (!std::isalnum(c)) return false;
        }
        return true;
    }

    bool isEmptyOrWhitespace(const std::string& str) {
        return trim(str).empty();
    }

    // ==================== RANDOM UTILITIES ====================
    int randomInt(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    double randomDouble(double min, double max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    std::string randomName() {
        static const std::vector<std::string> firstNames = {
            "James", "Mary", "John", "Patricia", "Robert", "Jennifer",
            "Michael", "Linda", "William", "Elizabeth", "David", "Barbara",
            "Richard", "Susan", "Joseph", "Jessica", "Thomas", "Sarah"
        };
        
        static const std::vector<std::string> lastNames = {
            "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
            "Miller", "Davis", "Rodriguez", "Martinez", "Hernandez", "Lopez",
            "Gonzalez", "Wilson", "Anderson", "Thomas", "Taylor", "Moore"
        };
        
        return firstNames[randomInt(0, firstNames.size() - 1)] + " " + 
               lastNames[randomInt(0, lastNames.size() - 1)];
    }

    std::string randomAddress() {
        static const std::vector<std::string> streets = {
            "Main St", "Oak Ave", "Maple Dr", "Cedar Ln", "Elm St", "Pine Rd",
            "Washington St", "Park Ave", "Lake St", "Hill Rd"
        };
        
        static const std::vector<std::string> cities = {
            "New York", "Los Angeles", "Chicago", "Houston", "Phoenix",
            "Philadelphia", "San Antonio", "San Diego", "Dallas", "San Jose"
        };
        
        return std::to_string(randomInt(100, 9999)) + " " +
               streets[randomInt(0, streets.size() - 1)] + ", " +
               cities[randomInt(0, cities.size() - 1)];
    }

    std::string randomOccupation() {
        static const std::vector<std::string> occupations = {
            "Doctor", "Engineer", "Teacher", "Nurse", "Accountant", "Manager",
            "Salesperson", "Driver", "Chef", "Artist", "Writer", "Musician",
            "Police Officer", "Firefighter", "Soldier", "Pilot", "Scientist"
        };
        return occupations[randomInt(0, occupations.size() - 1)];
    }

    std::string randomCaseTitle() {
        static const std::vector<std::string> adjectives = {
            "Mysterious", "Stolen", "Missing", "Secret", "Hidden", "Forgotten",
            "Ancient", "Valuable", "Dangerous", "Strange", "Curious", "Unsolved"
        };
        
        static const std::vector<std::string> nouns = {
            "Diamond", "Document", "Painting", "Jewelry", "Weapon", "Evidence",
            "Case", "Mystery", "Incident", "Affair", "Situation", "Puzzle"
        };
        
        return adjectives[randomInt(0, adjectives.size() - 1)] + " " +
               nouns[randomInt(0, nouns.size() - 1)];
    }

    std::string generateUUID() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 15);
        static std::uniform_int_distribution<> dis2(8, 11);
        
        std::stringstream ss;
        ss << std::hex;
        for (int i = 0; i < 8; i++) ss << dis(gen);
        ss << "-";
        for (int i = 0; i < 4; i++) ss << dis(gen);
        ss << "-4";
        for (int i = 0; i < 3; i++) ss << dis(gen);
        ss << "-";
        ss << dis2(gen);
        for (int i = 0; i < 3; i++) ss << dis(gen);
        ss << "-";
        for (int i = 0; i < 12; i++) ss << dis(gen);
        
        return ss.str();
    }

    // ==================== FORMATTING UTILITIES ====================
    std::string formatPercentage(double value) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << value << "%";
        return ss.str();
    }

    std::string formatCurrency(double amount) {
        std::stringstream ss;
        ss << "$" << std::fixed << std::setprecision(2) << amount;
        return ss.str();
    }

    std::string formatFileSize(size_t bytes) {
        const char* sizes[] = {"B", "KB", "MB", "GB"};
        int order = 0;
        double size = bytes;
        
        while (size >= 1024 && order < 3) {
            order++;
            size /= 1024;
        }
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << size << " " << sizes[order];
        return ss.str();
    }

    std::string padRight(const std::string& str, size_t length, char padChar) {
        if (str.length() >= length) return str;
        return str + std::string(length - str.length(), padChar);
    }

    std::string padLeft(const std::string& str, size_t length, char padChar) {
        if (str.length() >= length) return str;
        return std::string(length - str.length(), padChar) + str;
    }

    std::string centerString(const std::string& str, size_t length, char padChar) {
        if (str.length() >= length) return str;
        
        size_t padLength = length - str.length();
        size_t leftPad = padLength / 2;
        size_t rightPad = padLength - leftPad;
        
        return std::string(leftPad, padChar) + str + std::string(rightPad, padChar);
    }

    // ==================== INVESTIGATION-SPECIFIC UTILITIES ====================
    double calculateProbability(double evidenceWeight, int evidenceCount) {
        // Simple probability calculation based on evidence
        double baseProbability = evidenceWeight * evidenceCount;
        return clamp(baseProbability, 0.0, 100.0);
    }

    std::string generateCaseId(int sequence) {
        std::stringstream ss;
        ss << "CASE-" << std::setw(6) << std::setfill('0') << sequence;
        return ss.str();
    }

    std::string generateSuspectId(int sequence) {
        std::stringstream ss;
        ss << "SUSP-" << std::setw(6) << std::setfill('0') << sequence;
        return ss.str();
    }

    std::string generateEvidenceId(int sequence) {
        std::stringstream ss;
        ss << "EVID-" << std::setw(6) << std::setfill('0') << sequence;
        return ss.str();
    }

    std::string assessRiskLevel(double suspicionPercentage) {
        if (suspicionPercentage < 25) return "Low";
        if (suspicionPercentage < 50) return "Medium";
        if (suspicionPercentage < 75) return "High";
        return "Very High";
    }

    std::string getPriorityColor(const std::string& priority) {
        if (priority == "LOW") return "🟢";
        if (priority == "MEDIUM") return "🟡";
        if (priority == "HIGH") return "🟠";
        if (priority == "URGENT") return "🔴";
        return "⚪";
    }

    std::string getStatusIcon(const std::string& status) {
        if (status == "OPEN") return "🔍";
        if (status == "SOLVED") return "✅";
        if (status == "CLOSED") return "🔒";
        return "❓";
    }

    std::vector<std::string> generateInvestigationSteps(const std::string& caseType) {
        std::vector<std::string> steps;
        steps.push_back("1. Secure the crime scene");
        steps.push_back("2. Collect physical evidence");
        steps.push_back("3. Interview witnesses");
        steps.push_back("4. Identify potential suspects");
        steps.push_back("5. Analyze evidence");
        steps.push_back("6. Conduct follow-up interviews");
        steps.push_back("7. Build case file");
        steps.push_back("8. Present findings");
        return steps;
    }

    std::string calculateTimeSinceIncident(const std::chrono::system_clock::time_point& incidentTime) {
        auto now = std::chrono::system_clock::now();
        auto duration = now - incidentTime;
        
        auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count();
        auto days = hours / 24;
        
        if (days > 0) {
            return std::to_string(days) + " days ago";
        } else if (hours > 0) {
            return std::to_string(hours) + " hours ago";
        } else {
            return "Less than an hour ago";
        }
    }

    // ==================== FILE UTILITIES ====================
    bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }

    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    bool writeFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        
        file << content;
        return true;
    }

    bool appendToFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) return false;
        
        file << content;
        return true;
    }

    std::vector<std::string> readLines(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return {};
        
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        return lines;
    }

    bool writeLines(const std::string& filename, const std::vector<std::string>& lines) {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        
        for (const auto& line : lines) {
            file << line << "\n";
        }
        return true;
    }

    // ==================== MATHEMATICAL UTILITIES ====================
    double normalize(double value, double min, double max) {
        if (max == min) return 0.0;
        return (value - min) / (max - min);
    }

    double clamp(double value, double min, double max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    double lerp(double a, double b, double t) {
        return a + t * (b - a);
    }

    double calculateAverage(const std::vector<double>& values) {
        if (values.empty()) return 0.0;
        
        double sum = 0.0;
        for (double value : values) sum += value;
        return sum / values.size();
    }

    double calculateStandardDeviation(const std::vector<double>& values) {
        if (values.empty()) return 0.0;
        
        double mean = calculateAverage(values);
        double sumSquaredDiff = 0.0;
        
        for (double value : values) {
            double diff = value - mean;
            sumSquaredDiff += diff * diff;
        }
        
        return std::sqrt(sumSquaredDiff / values.size());
    }

    int calculateMedian(const std::vector<int>& values) {
        if (values.empty()) return 0;
        
        std::vector<int> sorted = values;
        std::sort(sorted.begin(), sorted.end());
        
        size_t mid = sorted.size() / 2;
        if (sorted.size() % 2 == 0) {
            return (sorted[mid - 1] + sorted[mid]) / 2;
        } else {
            return sorted[mid];
        }
    }

    // ==================== CONVERSION UTILITIES ====================
    int stringToInt(const std::string& str, int defaultValue) {
        try {
            return std::stoi(str);
        } catch (...) {
            return defaultValue;
        }
    }

    double stringToDouble(const std::string& str, double defaultValue) {
        try {
            return std::stod(str);
        } catch (...) {
            return defaultValue;
        }
    }

    bool stringToBool(const std::string& str) {
        std::string lowerStr = toLower(trim(str));
        return lowerStr == "true" || lowerStr == "1" || lowerStr == "yes" || lowerStr == "on";
    }

    std::string intToString(int value) {
        return std::to_string(value);
    }

    std::string doubleToString(double value, int precision) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << value;
        return ss.str();
    }

    std::string boolToString(bool value) {
        return value ? "true" : "false";
    }

    // ==================== DEBUG UTILITIES ====================
    void logInfo(const std::string& message) {
        std::cout << "[INFO] " << getCurrentDateTime() << " - " << message << std::endl;
    }

    void logWarning(const std::string& message) {
        std::cout << "[WARNING] " << getCurrentDateTime() << " - " << message << std::endl;
    }

    void logError(const std::string& message) {
        std::cerr << "[ERROR] " << getCurrentDateTime() << " - " << message << std::endl;
    }

    void logDebug(const std::string& message) {
        #ifdef DEBUG
        std::cout << "[DEBUG] " << getCurrentDateTime() << " - " << message << std::endl;
        #endif
    }

    std::string getCallStack() {
        // Simplified call stack - in real implementation, use platform-specific methods
        return "Call stack not available in this implementation";
    }

    void performanceTest(const std::string& testName, std::function<void()> testFunction) {
        auto start = std::chrono::high_resolution_clock::now();
        testFunction();
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Performance test '" << testName << "' took " << duration.count() << " ms" << std::endl;
    }

} // namespace DetectiveUtils