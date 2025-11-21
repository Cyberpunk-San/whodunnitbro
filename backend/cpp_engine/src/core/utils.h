#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <functional>  // ADD THIS LINE
#include <regex>       // ADD THIS LINE
#include <cmath>  
namespace DetectiveUtils {

    // ==================== STRING UTILITIES ====================
    std::string toUpper(const std::string& str);
    std::string toLower(const std::string& str);
    std::string trim(const std::string& str);
    std::string capitalizeWords(const std::string& str);
    bool containsIgnoreCase(const std::string& str, const std::string& substr);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    bool startsWith(const std::string& str, const std::string& prefix);
    bool endsWith(const std::string& str, const std::string& suffix);
    std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);

    // ==================== TIME & DATE UTILITIES ====================
    std::string getCurrentDateTime();
    std::string formatTimePoint(const std::chrono::system_clock::time_point& tp);
    std::chrono::system_clock::time_point stringToTimePoint(const std::string& timeStr);
    int daysBetween(const std::chrono::system_clock::time_point& from, 
                   const std::chrono::system_clock::time_point& to);
    bool isRecent(const std::chrono::system_clock::time_point& timePoint, int daysThreshold = 7);

    // ==================== VALIDATION UTILITIES ====================
    bool isValidName(const std::string& name);
    bool isValidEmail(const std::string& email);
    bool isValidPhoneNumber(const std::string& phone);
    bool isValidDate(const std::string& date);
    bool isStrongPassword(const std::string& password);
    bool isNumeric(const std::string& str);
    bool isAlphaNumeric(const std::string& str);
    bool isEmptyOrWhitespace(const std::string& str);

    // ==================== RANDOM UTILITIES ====================
    int randomInt(int min, int max);
    double randomDouble(double min, double max);
    std::string randomName();
    std::string randomAddress();
    std::string randomOccupation();
    std::string randomCaseTitle();
    std::string generateUUID();

    // ==================== FORMATTING UTILITIES ====================
    std::string formatPercentage(double value);
    std::string formatCurrency(double amount);
    std::string formatFileSize(size_t bytes);
    std::string padRight(const std::string& str, size_t length, char padChar = ' ');
    std::string padLeft(const std::string& str, size_t length, char padChar = ' ');
    std::string centerString(const std::string& str, size_t length, char padChar = ' ');

    // ==================== INVESTIGATION-SPECIFIC UTILITIES ====================
    double calculateProbability(double evidenceWeight, int evidenceCount);
    std::string generateCaseId(int sequence);
    std::string generateSuspectId(int sequence);
    std::string generateEvidenceId(int sequence);
    std::string assessRiskLevel(double suspicionPercentage);
    std::string getPriorityColor(const std::string& priority);
    std::string getStatusIcon(const std::string& status);
    std::vector<std::string> generateInvestigationSteps(const std::string& caseType);
    std::string calculateTimeSinceIncident(const std::chrono::system_clock::time_point& incidentTime);

    // ==================== FILE UTILITIES ====================
    bool fileExists(const std::string& filename);
    std::string readFile(const std::string& filename);
    bool writeFile(const std::string& filename, const std::string& content);
    bool appendToFile(const std::string& filename, const std::string& content);
    std::vector<std::string> readLines(const std::string& filename);
    bool writeLines(const std::string& filename, const std::vector<std::string>& lines);

    // ==================== MATHEMATICAL UTILITIES ====================
    double normalize(double value, double min, double max);
    double clamp(double value, double min, double max);
    double lerp(double a, double b, double t);
    double calculateAverage(const std::vector<double>& values);
    double calculateStandardDeviation(const std::vector<double>& values);
    int calculateMedian(const std::vector<int>& values);

    // ==================== CONVERSION UTILITIES ====================
    int stringToInt(const std::string& str, int defaultValue = 0);
    double stringToDouble(const std::string& str, double defaultValue = 0.0);
    bool stringToBool(const std::string& str);
    std::string intToString(int value);
    std::string doubleToString(double value, int precision = 2);
    std::string boolToString(bool value);

    // ==================== DEBUG UTILITIES ====================
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);
    void logDebug(const std::string& message);
    std::string getCallStack();
    void performanceTest(const std::string& testName, std::function<void()> testFunction);

} // namespace DetectiveUtils

#endif // UTILS_H