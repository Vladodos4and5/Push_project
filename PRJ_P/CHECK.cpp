#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <locale>
#include <codecvt>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

struct SVGFile {
    std::wstring filename;
    int width;
};

std::wstring readFile(const std::wstring& filePath) {
    std::wifstream file(filePath);
    file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    if (!file.is_open()) {
        std::wcerr << L"Ошибка: не удалось открыть файл " << filePath << std::endl;
        return L"";
    }
    std::wstringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void writeFile(const std::wstring& filePath, const std::wstring& content) {
    std::wofstream file(filePath);
    file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    if (!file.is_open()) {
        std::wcerr << L"Ошибка: не удалось записать файл " << filePath << std::endl;
        return;
    }
    file << content;
}

std::wstring shiftSVG(const std::wstring& svgContent, int xShift) {
    size_t pos = svgContent.find(L"<svg");
    if (pos == std::wstring::npos) {
        std::wcerr << L"Ошибка: некорректное содержимое SVG (отсутствует <svg>)" << std::endl;
        return L"";
    }
    return L"<g transform=\"translate(" + std::to_wstring(xShift) + L",0)\">\n" + svgContent.substr(pos) + L"\n</g>\n";
}

void combineSVGFiles(const std::vector<SVGFile>& svgFiles, const std::wstring& outputFilename, const std::wstring& connectorFilename, int connectorWidth) {
    std::wstring resultSVG = L"<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";
    int currentX = 0;

    std::wcout << L"Файлы в порядке объединения:\n";
    for (size_t i = 0; i < svgFiles.size(); ++i) {
        std::wcout << L" " << i << L": " << svgFiles[i].filename << std::endl;
        std::wstring letterContent = readFile(svgFiles[i].filename);
        if (letterContent.empty()) continue;

        resultSVG += shiftSVG(letterContent, currentX);
        currentX += svgFiles[i].width;

        if (i < svgFiles.size() - 1 && svgFiles[i].filename != L"emp.svg" && svgFiles[i + 1].filename != L"emp.svg") {
            std::wstring connectorContent = readFile(connectorFilename);
            if (!connectorContent.empty()) {
                resultSVG += shiftSVG(connectorContent, currentX);
                currentX += connectorWidth;
            }
        }
    }

    resultSVG += L"</svg>\n";
    writeFile(outputFilename, resultSVG);
    std::wcout << L"SVG-файл успешно создан: " << outputFilename << std::endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U16TEXT);

    std::wstring input;
    std::wcout << L"Введите текст: ";
    _setmode(_fileno(stdin), _O_U16TEXT);  // настройка для wcin
    std::getline(std::wcin, input);

    std::map<wchar_t, std::wstring> mapping = {
        {L'п', L"p.svg"},
        {L'о', L"o.svg"},
        {L'а', L"a1.svg"},
        {L'б', L"b.svg"},
        {L'в', L"v.svg"},
        {L'г', L"g.svg"},
        {L'д', L"d.svg"},
        {L'е', L"e2.svg"},
        {L'ё', L"e2.svg"},
        {L'ж', L"zh.svg"},
        {L'з', L"z.svg"},
        {L'и', L"i.svg"},
        {L'й', L"ykr.svg"},
        {L'к', L"k.svg"},
        {L'л', L"l.svg"},
        {L'м', L"m.svg"},
        {L'н', L"n.svg"},
        {L'р', L"r.svg"},
        {L'с', L"c.svg"},
        {L'т', L"t.svg"},
        {L'у', L"u.svg"},
        {L'ф', L"f.svg"},
        {L'х', L"h.svg"},
        {L'ц', L"ts.svg"},
        {L'ч', L"che.svg"},
        {L'ш', L"sh.svg"},
        {L'щ', L"shi.svg"},
        {L'ъ', L"tvz.svg"},
        {L'ы', L"ykr.svg"},
        {L'ь', L"mz.svg"},
        {L'э', L"e.svg"},
        {L'ю', L"uy.svg"},
        {L'я', L"ua.svg"},
        {L',', L"zpt.svg"},
        {L'.', L"tchk.svg"},
        {L' ', L"emp.svg"},
        {L'А', L"aZ.svg"},
        {L'Б', L"bZ.svg"},
        {L'В', L"vZ.svg"},
        {L'Г', L"gZ.svg"},
        {L'Е', L"eZ.svg"},
        {L'Ё', L"euoZ.svg"},
        {L'Ж', L"zheZ.svg"},
        {L'З', L"zZ.svg"},
        {L'И', L"iZ.svg"},
        {L'Й', L"ykrZ.svg"},
        {L'К', L"kZ.svg"},
        {L'М', L"m_soundZ.svg"},
        {L'Н', L"nZ.svg"},
        {L'О', L"oZ.svg"},
        {L'П', L"pZ.svg"},
        {L'С', L"sZ.svg"},
        {L'Т', L"tZ.svg"},
        {L'У', L"uZ.svg"},
        {L'Ф', L"fZ.svg"},
        {L'Ц', L"tsZ.svg"},
        {L'Ч', L"cheZ.svg"},
        {L'Ш', L"shZ.svg"},
        {L'Щ', L"shiZ.svg"},
        {L'Ы', L"ieZ.svg"},
        {L'Ь', L"mzZ.svg"},
        {L'Ю', L"yuZ.svg"},
        {L'Э', L"e2Z.svg"},
        {L'Я', L"yuaZ.svg"},
        {L'Л', L"IZ.svg"},
        {L'Ъ', L"tvZ.svg"}
    };

    std::vector<SVGFile> svgFiles;
    int letterWidth = 25;
    int capitalLetterWidth = 28;  // 👈 можешь потом сделать по-другому, если нужно точнее
    int connectorWidth = 55;
    int spaceWidth = 80;

    for (wchar_t ch : input) {
        if (mapping.find(ch) != mapping.end()) {
            std::wstring filename = mapping[ch];
            int width = (filename == L"emp.svg") ? spaceWidth :
                (iswupper(ch) ? capitalLetterWidth : letterWidth);
            svgFiles.push_back({ filename, width });
        }
        else {
            std::wcerr << L"Неизвестный символ: " << ch << std::endl;
        }
    }

    std::wstring connectorFile = L"sd.svg";
    combineSVGFiles(svgFiles, L"_____________THIS___________.svg", connectorFile, connectorWidth);

    return 0;
}
