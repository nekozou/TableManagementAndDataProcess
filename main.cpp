#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

// 定义表格数据类型
typedef vector< vector <string> > Table;

// 全局变量，存储当前表格数据
Table currentTable;

// 功能菜单
void showMenu() {
    cout << "=== 表格管理/处理功能菜单 ===" << endl;
    cout << "F1. 显示表格" << endl;
    cout << "F2. 建立空白表格" << endl;
    cout << "F3. 输入数据" << endl;
    cout << "F4. 复制数据块" << endl;
    cout << "F5. 鼠标操作" << endl;
    cout << "F6. 汉字显示/输入" << endl;
    cout << "F7. 单元格输入公式" << endl;
    cout << "F8. 统计计算" << endl;
    cout << "F9. 统计计算方式" << endl;
    cout << "F10. 排序" << endl;
    cout << "F11. 保存表格" << endl;
    cout << "=========================" << endl;
}

// 显示表格
void showTable() {
    if (currentTable.empty()) {
        cout << "表格为空！" << endl;
        return;
    }

    cout << "表格内容：" << endl;
    for (const auto& row : currentTable) {
        for (const auto& cell : row) {
            cout << cell << "\t";
        }
        cout << endl;
    }
}

// 建立空白表格
void createTable(int rows, int cols) {
    currentTable.clear();
    currentTable.resize(rows, vector<string>(cols, ""));
    cout << "成功创建空白表格！" << endl;
}

// 输入数据
void inputData(int row, int col, const string& data) {
    if (row < 0 || row >= currentTable.size() || col < 0 || col >= currentTable[0].size()) {
        cout << "输入的行或列超出范围！" << endl;
        return;
    }

    currentTable[row][col] = data;
    cout << "成功输入数据！" << endl;
}

// 复制数据块
void copyDataBlock(int srcRow, int srcCol, int destRow, int destCol, char mode) {
    if (srcRow < 0 || srcRow >= currentTable.size() || srcCol < 0 || srcCol >= currentTable[0].size()
        || destRow < 0 || destRow >= currentTable.size() || destCol < 0 || destCol >= currentTable[0].size()) {
        cout << "输入的行或列超出范围！" << endl;
        return;
    }

    if (mode == 'R') { // 代替
        currentTable[destRow][destCol] = currentTable[srcRow][srcCol];
    } else if (mode == 'A') { // 相加
        int num1 = stoi(currentTable[destRow][destCol]);
        int num2 = stoi(currentTable[srcRow][srcCol]);
        currentTable[destRow][destCol] = to_string(num1 + num2);
    } else if (mode == 'S') { // 相减
        int num1 = stoi(currentTable[destRow][destCol]);
        int num2 = stoi(currentTable[srcRow][srcCol]);
        currentTable[destRow][destCol] = to_string(num1 - num2);
    } else if (mode == 'C') { // 按条件替换
        string condition = currentTable[srcRow][srcCol];
        for (auto& row : currentTable) {
            for (auto& cell : row) {
                if (cell == condition) {
                    cell = currentTable[srcRow][srcCol];
                }
            }
        }
    } else {
        cout << "无效的复制方式！" << endl;
        return;
    }

    cout << "成功复制数据块！" << endl;
}

// 统计计算
void calculate(char mode) {
    if (currentTable.empty()) {
        cout << "表格为空！" << endl;
        return;
    }

    if (mode == 'T') { // 合计
        double sum = 0;
        for (const auto& row : currentTable) {
            for (const auto& cell : row) {
                sum += stod(cell);
            }
        }
        cout << "合计值为：" << sum << endl;
    } else if (mode == 'A') { // 求平均
        double sum = 0;
        int count = 0;
        for (const auto& row : currentTable) {
            for (const auto& cell : row) {
                sum += stod(cell);
                count++;
            }
        }
        double average = sum / count;
        cout << "平均值为：" << average << endl;
    } else if (mode == 'M') { // 求最大值
        double maxVal = stod(currentTable[0][0]);
        for (const auto& row : currentTable) {
            for (const auto& cell : row) {
                double val = stod(cell);
                if (val > maxVal) {
                    maxVal = val;
                }
            }
        }
        cout << "最大值为：" << maxVal << endl;
    } else if (mode == 'N') { // 求最小值
        double minVal = stod(currentTable[0][0]);
        for (const auto& row : currentTable) {
            for (const auto& cell : row) {
                double val = stod(cell);
                if (val < minVal) {
                    minVal = val;
                }
            }
        }
        cout << "最小值为：" << minVal << endl;
    } else {
        cout << "无效的统计计算方式！" << endl;
        return;
    }
}

// 排序
void sortTable(int row, int col, bool ascending, bool caseSensitive) {
    if (row < 0 || row >= currentTable.size() || col < 0 || col >= currentTable[0].size()) {
        cout << "输入的行或列超出范围！" << endl;
        return;
    }

    sort(currentTable.begin(), currentTable.end(), [&](const vector<string>& row1, const vector<string>& row2) {
        string val1 = row1[col];
        string val2 = row2[col];
        if (!caseSensitive) {
            transform(val1.begin(), val1.end(), val1.begin(), ::tolower);
            transform(val2.begin(), val2.end(), val2.begin(), ::tolower);
        }
        return ascending ? val1 < val2 : val1 > val2;
    });

    cout << "成功排序表格！" << endl;
}

// 保存表格
void saveTable(const string& filename) {
    ofstream outputFile(filename);
    if (!outputFile) {
        cout << "无法保存表格！" << endl;
        return;
    }

    for (const auto& row : currentTable) {
        for (const auto& cell : row) {
            outputFile << cell << ",";
        }
        outputFile << endl;
    }

    outputFile.close();
    cout << "成功保存表格至文件：" << filename << endl;
}

int main() {
    int rows, cols;
    int srcRow, srcCol, destRow, destCol;
    char mode;
    string data, filename;

    showMenu();
    while (true) {
        char choice;
        cout << "请输入功能编号：";
        cin >> choice;

        switch (choice) {
            case '1':
                showTable();
                break;
            case '2':
                cout << "请输入表格的行数和列数：";
                cin >> rows >> cols;
                createTable(rows, cols);
                break;
            case '3':
                cout << "请输入要输入数据的行号、列号和数据：";
                cin >> srcRow >> srcCol >> data;
                inputData(srcRow, srcCol, data);
                break;
            case '4':
                cout << "请输入源数据块的行号、列号和目标数据块的行号、列号：";
                cin >> srcRow >> srcCol >> destRow >> destCol;
                cout << "请输入数据复制方式（R代替，A相加，S相减，C按条件替换）：";
                cin >> mode;
                copyDataBlock(srcRow, srcCol, destRow, destCol, mode);
                break;
            case '5':
                cout << "不支持鼠标操作！" << endl;
                break;
            case '6':
                cout << "不支持汉字显示/输入！" << endl;
                break;
            case '7':
                cout << "不支持公式输入！" << endl;
                break;
            case '8':
                cout << "请输入统计计算方式（T合计，A求平均，M求最大值，N求最小值）：";
                cin >> mode;
                calculate(mode);
                break;
            case '9':
                cout << "不支持统计计算方式！" << endl;
                break;
            case '10':
                cout << "请输入要排序的行号或列号：";
                cin >> srcRow >> srcCol;
                cout << "请输入排序方式（A升序，D降序）：";
                cin >> mode;
                cout << "是否区分大小写（Y是，N否）：";
                cin >> choice;
                sortTable(srcRow, srcCol, mode == 'A', choice == 'Y');
                break;
            case '11':
                cout << "请输入要保存的文件名：";
                cin >> filename;
                saveTable(filename);
                break;
            default:
                cout << "无效的功能编号！" << endl;
                break;
        }

        cout << endl;
        showMenu();
    }

    return 0;
}