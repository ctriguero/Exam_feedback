#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Student {
    std::string name;
    std::string surname;
    double mark;
    double M[13];
};

// Split a TAB-separated line
std::vector<std::string> splitTab(const std::string &line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '\t')) {
        result.push_back(token);
    }
    return result;
}

int main() {
    std::ifstream infile("students.dat");
    if (!infile) {
        std::cerr << "Error opening students.dat\n";
        return 1;
    }

    std::vector<Student> students;
    std::string line;

    while (std::getline(infile, line)) {
        if (line.empty()) continue;

        std::vector<std::string> t = splitTab(line);
        if (t.size() != 16) {
            std::cerr << "Invalid line (expected 16 fields): " << line << "\n";
            continue;
        }

        Student s;
        s.name = t[0];
        s.surname = t[1];
        s.mark = std::stod(t[2]);
        for (int i = 0; i < 13; ++i) {
            s.M[i] = std::stod(t[i + 3]);
        }

        students.push_back(s);
    }

    std::ofstream out("all_students.tex");

    // ---------- LaTeX preamble ----------
    out << "\\documentclass{article}\n";
    out << "\\usepackage{tikz}\n";
    out << "\\usepackage{pgfplots}\n";
    out << "\\pgfplotsset{compat=1.18}\n";
    out << "\\usepackage{pgfplotstable}\n";
    out << "\\usepackage{float}\n";
    out << "\\usepackage{graphicx}\n";
    out << "\\usepackage[ left=1.0cm,right=1.0cm,top=1.5cm,bottom=1.5cm ]{geometry}\n";
    out << "\\begin{document}\n";

    // ---------- One section per student ----------
    for (size_t i = 0; i < students.size(); ++i) {
        const Student &s = students[i];

        out << "\\thispagestyle{empty}\n";
        out << "\\section*{\\huge AS B Physics examination feedback}\n";
        out << "\\section*{" << s.name << " " << s.surname
            << " \\hfill " << s.mark << " \\%}\n";

        out << "\\section*{Class mark distribution. Year average 69\\%. Class average 66\\%.}\n";

        out << "\\begin{figure}[H]\n";
        out << "    \\centering\n";
        out << "    \\includegraphics[height=0.378\\linewidth]{f1.pdf}"
               "\\hfill"
               "\\includegraphics[height=0.378\\linewidth]{f2.pdf}\n";
        out << "\\end{figure}\n";

        out << "\\section*{Results by components}\n";
        out << "\\begin{table}[H]\n";
        out << "    \\centering\n";
        out << "    \\scalebox{1.3}{\n";
        out << "    \\begin{tabular}{|lll|}\\hline\n";
        out << "    \\% mark & Component & Action \\\\\n";
        out << "    \\hline\\hline\n";

        out << "    " << s.M[0]  << " & \\textbf{Physical Quantities:} & \\\\\n";
        out << "    " << s.M[1]  << " & \\textbf{Scalars and Vectors:} & \\\\\n";
        out << "    " << s.M[2]  << " & \\textbf{Moments:} & \\\\\n";
        out << "    " << s.M[3]  << " & \\textbf{Motion:} & \\\\\n";
        out << "    " << s.M[4]  << " & \\textbf{Projectile Motion:} & \\\\\n";
        out << "    " << s.M[5]  << " & \\textbf{Voltage, Current, Charge:} & \\\\\n";
        out << "    " << s.M[6]  << " & \\textbf{IV Characteristic Curves:} & \\\\\n";
        out << "    " << s.M[7]  << " & \\textbf{Resistivity:} & \\\\\n";
        out << "    " << s.M[8]  << " & \\textbf{Internal resistance:} & \\\\\n";
        out << "    " << s.M[9]  << " & \\textbf{Potential dividers:} & \\\\\n";
        out << "    " << s.M[10] << " & \\textbf{Experimental description:} & \\\\\n";
        out << "    \\hline\\hline\n";
        out << "    " << s.M[11] << " & \\textbf{Definitions:} & \\\\\n";
        out << "    & \\multicolumn{2}{l|}{Ideally, you should understand all definitions.} \\\\\n";
        out << "    " << s.M[12] << " & \\textbf{Conceptual understanding:} & \\\\\n";
        out << "    \\hline\n";
        out << "    \\end{tabular}}\n";
        out << "\\end{table}\n";

        if (i != students.size() - 1) {
            out << "\\newpage\n";
            out << "\\thispagestyle{empty}\n";
        }
    }

    out << "\\end{document}\n";
    out.close();

    std::cout << "Generated all_students.tex successfully\n";
    return 0;
}
