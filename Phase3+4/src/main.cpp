//**********************************************************************************************************************************************//
// 5 Function Calculator                                                                                                                        //
// Written as a technical examination for a position at VS Merlot.                                                                              //
// Programmed by Kennedy Ford.                                                                                                                  //
//                                                                                                                                              //
// The project was given to me at around 3pm on Monday, Feb. 12th, 2024.                                                                        //
// The time limit given was one week. I began this project (researching it and studying the document) that evening, and                         //
// I began programming it Tuesday, Feb. 13th, 2024. I have split the time for working on this between both the tasks given                      //
// from my current employer and the work load from my current classes.                                                                          //                                                         //
//                                                                                                                                              //
// A few additional notes:                                                                                                                      //
// 1. I have never used most of these tools. This is inlcuding C++, ImGui, and OpenGLFW                                                         //
// 2. This was built starting with a template example displaying the capabilities of ImGui and OpenGLFW.                                        //
//    The only code that I have reused is the code that creates the window. All other logic and visuals of the GUI were created by me.          //  
//    Any clarification for what code is mine and was has been recycled, I can answer on request. I have done my best to make sure that         //
//    the majority of the code written for this has been of my own doing                                                                        //
//                                                                                                                                              //
// Thank you for the opportunity to let me display my abilites. I look forward to demoing thia for you soon.                                    //
//      - Kennedy                                                                                                                               //                                                                                    
//**********************************************************************************************************************************************//


#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h>
using namespace std;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

//*********************************************//

// Simple addition function
float add(float a, float b)
{
    return a + b;
}

// Simple subtraction function
float subtract(float a, float b)
{
    return a - b;
}

// Simple multiplication function
float multiply(float a, float b)
{
    return a * b;
}

// Simple division function
float divide(float a, float b)
{
    return a / b;
}

// Takes the percent of tax (b) and divides it by 100 using our divide function to get the value 
// to a decimal representation of the given percentage. It then multiplies that value by the total (a) 
// which is what we need to add to the total (a).
float addTax(float a, float b)
{
    return add(a, (multiply(a, divide(b, 100))));
}

// Assigns the input (if the input is an integer) to the passed in toAssign variable (will either be a or b passed in).
// This function also accounts for whether or not the inputted value is being added to the left or the right of the decimal.
void assignInput(float toAssign, float* temp, float input, bool* increaseDB, int decimalBalance)
{
    if (decimalBalance == 1)
    {
        *temp = multiply(toAssign, 10) + input;
        *increaseDB = false;
    }
    else
    {
        *temp = toAssign + divide(input, decimalBalance); 
        *increaseDB = true;
    }

}

// Analyzes the operation that is inputted and performs the correct operations.
void executeOp(float a, float b, string op, float* result)
{
    if (op.compare("+") == 0)
    {
        *result = add(a, b);
    }
    else if (op.compare("-") == 0)
    {
        *result = subtract(a, b);
    }
    else if (op.compare("*") == 0)
    {
        *result = multiply(a, b);
    }
    else if (op.compare("/") == 0)
    {
        *result = divide(a, b);
    }
    else if (op.compare("%") == 0) 
    {
        *result = addTax(a, b);
    }
}

// *** MAIN ***
int main(int, char**)
{
    //******************************************************* VARIBALE INITIALIZATION & DEFAULT ASSIGNMENTS *****************************************************************//
    // BOOLEAN VARIABLES                                                                                                                                                     //
    // - DEBUG: A varibable used for toggling cout statements to help with logical troubleshooting. Will be left off unless otherwise changed.                               //
    // - aDefined, bDefined, opDefined: Used to keep up with whether or not the primary variables ('a', 'b', and 'op') are assigned.                                         //
    // - numInputted, opInputted: Used for clarification to determine what type of input was just selected by the user.                                                      //
    // - inceaseDB: Used to determine whether or not the input from the user is being assigned to the right of the decimal point or not.                                     //
    // - negativeA, negativeB: Defines if the variables 'a' or 'b' are currently negative.                                                                                   //
    // - traceOn: Determines whether the logic trace should be outputted to the trace logic tab. Can be toggled using the "Intro Trace On"                                   //
    //   and "Intro Trace Off" buttons.                                                                                                                                      //
    //                                                                                                                                                                       //
    // STRING VARIABLES:                                                                                                                                                     //
    // - op: Stores the operation that is first in line to be executed.                                                                                                      //
    // - nextOp: Stores the operation that is next/second in line to be executed.                                                                                            //
    //                                                                                                                                                                       //
    // FLOAT VARIABLES:                                                                                                                                                      //
    // - a, b: Used as our two operands. The current result will often be stored in 'a'.                                                                                     //
    // - tempA, tempB: Used as inputs to specific functions as a way to return multiple computed values from a function. (This is kind of funky I know, forgive me.)         //
    // - prevResult: Stores the previously computed result (the one computed before the currently computed value residing within 'a'). This is used to revert                //
    //   the state of the system back to the previous result when the 'CE' button is selected.                                                                               //
    // - input: The variable used to store the most recent numeric input from the user (button wise, not the entire number).                                                 //
    // - decimalBalance: Used to balance a given numeric input if the user is inputting decimal values.                                                                      //
    // - ListBox: What is displayed to the user, which is either the current result or the last inputted value based on if '=' has been selected.                            //
    //   ('a' and 'b' are also displayed in this implementation, but if there was only one number displayed, it would be the value stored in the 'ListBox')                  //
    // - decisionCount: The current tally of how many logically significant decisions have been made by the system.                                                          //
    //                                                                                                                                                                       //
    // CHAR & CHAR* VARIABLES                                                                                                                                                //
    // - testStart: used as a memory buffer and the start of the trace text box. (For some reason, the GUI makes the system crash if this is not in place.)                  //
    // - text: The text of the trace that is displayed on the 'Trace Logic" tab. This is what is appended after every significant logic decision was made.                   //
    //***********************************************************************************************************************************************************************//

    bool DEBUG = false;

    string op, nextOp;
    float a, b, prevResult, tempA, tempB, input, decimalBalance, ListBox;
    bool aDefined, bDefined, opDefined, increaseDB, numInputted, opInputted, traceOn, negativeA, negativeB;
    
    decimalBalance = 1;
    ListBox = 0;
    a = 0;
    b = 0;

    char textStart[4096 * 64] = "";
    char* text = strcat(textStart, "Trace:\n");
    float decisionCount = 0;

    aDefined = false;
    bDefined = false;
    opDefined = false;
    numInputted = false;
    opInputted = false;
    traceOn = false;
    negativeA = false;
    negativeB = false;

    // creating the CalcTrace.txt file
    ofstream MyFile("CalcTrace.txt"); 

//*********************************************//

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(400, 500, "5 Function Calculator", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_window)
        {
            ImGui::ShowDemoWindow(&show_window);
        }

    //*********************************************//

        // *** GUI DESIGN & LOGIC STARTS HERE ***
        {
            if (ImGui::TreeNode("5 Function Calculator"))
            {
                if (ImGui::BeginTabBar("MyTabBar"))
                {
                    if (ImGui::BeginTabItem("Calculator"))
                    {
                        ImGui::Text("ListBox: %f", ListBox);
                        ImGui::Text("a: %f", a);
                        ImGui::Text("b: %f", b);
                        // ImGui::TextWrapped("Memo: %d", memoValue);
                        ImGui::Spacing();

                        //************************************************ COMMENTS FOR BUTTONS ************************************************************************//
                        // - All numbers (0-9): The variable input is set as the number that is selected                                                                //
                        //   and the numInputted boolean is switched to true.                                                                                           //
                        // - All 5 Function Buttons (+, -, *, /, %): If there is already an operation that has been assigned but not executed yet,                      //
                        //   then the operation is saved in the nextOp variable and then assigned to the op variable after the current operation is executed,           //
                        //   which should be in the same loop cycle. The subtraction button will assign both a negative to an input or will be assigned as an operation //
                        //   for execution based on the current state of the system.                                                                                    //
                        // - Equals (=): Executes the op that has been selected (if applicable) and resets many of the variables to prepare for future inputs.          //
                        // - Clearing Buttons (CE & C): for CE, if an operation hasn't just been performed, then the current total is presented in the list box.        //
                        //   otherwise, the previous result (the result of the previous operation and not the current one) is returned to the ListBox. For C,           //
                        //   All primary and relevant variables are reset to their default states.                                                                      //
                        //**********************************************************************************************************************************************//


                        ///////////////
                        // FIRST ROW //
                        ///////////////

                        // *** Trace on ***
                        if (ImGui::Button("Intro Trace On", ImVec2 (110, 30)))
                        {   
                            traceOn = true;
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: Intro Trace On\n");
                                decisionCount = decisionCount + 1;
                            }
                        }
                        ImGui::SameLine();

                        if (ImGui::Button("Intro Trace Off", ImVec2 (115, 30)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: Intro Trace Off\n");
                                decisionCount = decisionCount + 1;
                            }
                            traceOn = false;
                        }


                        /////////////
                        // NEW ROW //
                        /////////////

                        // *** 7 ***
                        ImGui::SeparatorText("");
                        if (ImGui::Button("7", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '7' selected as input\n");
                                decisionCount = decisionCount + 1;
                            }

                            input = 7;
                            numInputted = true;
                        }
                        ImGui::SameLine();

                        // *** 8 ***
                        if (ImGui::Button("8", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '8' selected as input\n");
                                decisionCount = decisionCount + 1;
                            }

                           input = 8;
                           numInputted = true;
                        }
                        ImGui::SameLine();

                        // *** 9 ***
                        if (ImGui::Button("9", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '9' selected as input\n");
                                decisionCount = decisionCount + 1;
                            }

                           input = 9;
                           numInputted = true;
                        }
                        ImGui::SameLine();

                        // *** C ***
                        if (ImGui::Button("C", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: 'C' is selected\n");
                                decisionCount = decisionCount + 1;
                            }

                            ListBox = 0;
                            a = 0;
                            b = 0;
                            aDefined = false;
                            bDefined = false;
                            decimalBalance = 1;
                            op = "";
                        }
                        ImGui::SameLine();

                        // *** CE ***
                        if (ImGui::Button("CE", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: 'CE' is selected\n");
                                decisionCount = decisionCount + 1;
                            }

                            if (opDefined == 1)
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: 'Listbox' has been set to 'a'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                ListBox = a;
                            }
                            else
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: 'ListBox' has been set to 'prevResult'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                a = prevResult;
                                ListBox = a;   
                            }
                            b = 0;
                        }


                        /////////////
                        // NEW ROW //
                        /////////////


                        // *** 4 ***
                        if (ImGui::Button("4", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '4' selected as input\n");
                                decisionCount = decisionCount + 1;
                            }

                            input = 4;
                            numInputted = true;
                        }
                        ImGui::SameLine();

                        // *** 5 ***
                        if (ImGui::Button("5", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '5' selected as input\n");
                                decisionCount = decisionCount + 1;
                            }

                            input = 5;
                            numInputted = true;
                        }
                        ImGui::SameLine();

                        // *** 6 ***
                        if (ImGui::Button("6", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '6' selected as input\n");
                                decisionCount = decisionCount + 1;
                            }

                            input = 6;
                            numInputted = true;
                        }
                        ImGui::SameLine();
                        
                        // *** + ***
                        if (ImGui::Button("+", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '+' selected as the operation\n");
                                decisionCount = decisionCount + 1;
                            }

                            if (op.length() == 0)
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: '+' was assigned to 'op'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                op = "+";
                            }
                            else
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: '+' was assigned to 'nextOp'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                nextOp = "+";
                            }

                            opInputted = true;
                        }
                        ImGui::SameLine();

                        // *** - ***
                        if (ImGui::Button("-", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '-' selected as the operation\n");
                                decisionCount = decisionCount + 1;
                            }

                            if (op.length() == 0)
                            {
                                if (aDefined == 0)
                                {
                                    if (traceOn == 1)
                                    {
                                        text = strcat(text, "Decision Point: 'a' is now negative \n");
                                        decisionCount = decisionCount + 1;
                                    }
                                    negativeA = true;
                                    negativeB = false;
                                    opInputted = false;
                                }
                                else
                                {
                                    if (traceOn == 1)
                                    {
                                        text = strcat(text, "Decision Point: '-' was assigned to 'op'\n");
                                        decisionCount = decisionCount + 1;
                                    }
                                    op = "-";
                                    negativeA = false;
                                    negativeB = false;
                                    opInputted = true;
                                }
                            }
                            else
                            {
                                if ((opDefined == 1) & (b == 0))
                                {
                                    if (traceOn == 1)
                                    {
                                        text = strcat(text, "Decision Point: 'b' is now negative\n");
                                        decisionCount = decisionCount + 1;
                                    }
                                    negativeB = true;
                                    negativeA = false;
                                    opInputted = false;
                                }
                                else
                                {   
                                    if (traceOn == 1)
                                    {
                                        text = strcat(text, "Decision Point: '-' was assigned to 'nextOp'\n");
                                        decisionCount = decisionCount + 1;
                                    }
                                    nextOp = "-";
                                    negativeA = false;
                                    negativeB = false;
                                    opInputted = true;
                                }
                            }
                        }


                        /////////////
                        // NEW ROW //
                        /////////////


                        // *** 1 ***
                        if (ImGui::Button("1", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '1' selected as input\n");
                                decisionCount = decisionCount + 1;
                            }

                            input = 1;
                            numInputted = true;
                        }
                        ImGui::SameLine();

                        // *** 2 ***
                        if (ImGui::Button("2", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '2' selected as input\n");
                                decisionCount = decisionCount + 1;
                            }

                            input = 2;
                            numInputted = true;
                        }
                        ImGui::SameLine();

                        // *** 3 ***
                        if (ImGui::Button("3", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '3' selected as input\n");
                                decisionCount = decisionCount + 1;
                            }

                            input = 3;
                            numInputted = true;
                        }
                        ImGui::SameLine();
                        
                        // *** * *** (lol oh well im keeping it like this XD)
                        if (ImGui::Button("*", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '*' selected as the operation\n");
                                decisionCount = decisionCount + 1;
                            }

                            if (op.length() == 0)
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: '*' was assigned to 'op'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                op = "*";
                            }
                            else
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: '*' was assigned to 'nextOp'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                nextOp = "*";
                            }

                            opInputted = true;
                        }
                        ImGui::SameLine();

                        // *** / ***
                        if (ImGui::Button("/", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '/' selected as the operation\n");
                                decisionCount = decisionCount + 1;
                            }

                            if (op.length() == 0)
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: '/' was assigned to 'op'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                op = "/";
                            }
                            else
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: '/' was assigned to 'nextOp'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                nextOp = "/";
                            }

                            opInputted = true;
                        }


                        /////////////
                        // NEW ROW //
                        /////////////


                        // *** 0 ***
                        if (ImGui::Button("0", ImVec2 (128, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '0' selected as input\n");
                                decisionCount = decisionCount + 1;
                            }

                            input = 0;
                            numInputted = true;
                        }
                        ImGui::SameLine();
                        if (ImGui::Button(".", ImVec2 (60, 75)))
                        {
                            if (decimalBalance <= 1)
                            {
                                decimalBalance = 10;
                            }
                        }
                        ImGui::SameLine();
                        
                        // *** = ***
                        if (ImGui::Button("=", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '=' is selected\n");
                                decisionCount = decisionCount + 1;
                            }

                            if (nextOp.length() == 0)
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: 'op' was assigned to 'nextOp'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                nextOp = op;
                            }
                            
                            executeOp(a, b, nextOp, &tempA);
                            ListBox = tempA;
                            a = ListBox;
                            b = 0;

                            bDefined = false;
                            opDefined = false;

                            op = "";
                            nextOp = "";
                            
                            // inputs current info to CalcTrace.txt
                            MyFile << "Input: " << "=" << endl;
                            MyFile << "op: " << op << endl;
                            MyFile << "ListBox: " << ListBox << endl;
                            MyFile << "a: " << a << endl;
                            MyFile << "b: " << b << endl;
                            MyFile << "\n-----------\n\n";
                        }
                        ImGui::SameLine();

                        // *** % ***
                        if (ImGui::Button("%", ImVec2 (60, 75)))
                        {   
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: '%' selected as the operation\n");
                                decisionCount = decisionCount + 1;
                            }

                            if (op.length() == 0)
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: '%' was assigned to 'op'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                op = "%";
                            }
                            else
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: '%' was assigned to 'nextOp'\n");
                                    decisionCount = decisionCount + 1;
                                }
                                nextOp = "%";
                            }

                            opInputted = true;
                        }

                    
                        ////////////////////////////////////////
                        // *** ANALYSIS/CHANGES TO SYSTEM *** //
                        ////////////////////////////////////////

                        // if an operation has been inputted and there is already an operation defined (meaning there there have 
                        // been two operations selected), then the bDefined boolean is set to true.
                        if ((opInputted == 1) & (opDefined == 1))
                        {
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: 'bDefined' was set to true\n");
                                decisionCount = decisionCount + 1;
                            }

                            bDefined = true;
                        }

                        // if aDefined, bDefined, and opDefined are all true, then execute the current operation that has been selected.
                        // the internal code to this if statement is the same as the code from the equals button except for the fact that
                        // the prevResult is set and instead of clearing the op and nextOp variables, the operation held within nextOp
                        // is assigned to the variable op (to move it ahead in line and so it executes on the next time this condition is true).
                        if ((aDefined == 1) & (bDefined == 1) & (opDefined == 1))
                        {
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: An operation has been executed\n");
                                decisionCount = decisionCount + 1;
                            }

                            prevResult = a;
                            executeOp(a, b, op, &tempA);

                            a = tempA;
                            ListBox = b;
                            b = 0;

                            bDefined = false;
                            opDefined = false;

                            op = nextOp;

                            if (DEBUG == 1)
                            {
                                cout << "In all 1 if" << endl;
                                cout << "aDefined: " << aDefined << endl;
                                cout << "bDefined: " << bDefined << endl;
                                cout << "opDefined: " << opDefined << endl;
                            }
                        }

                        // if an operation has been inputted by the user, analyzes and edits the current state of the system.
                        // if 'a' hasn't been defined yet, then the signification of an operation being inputted means that it has, and
                        // then aDefined is set to true. This also means that an operation has been defined so the code also changes the 
                        // opDefined variable to true. Likewise, the system is moving on to a new inputted number. Thus, both the increaseDB
                        // and decimalBalance variables are reset to their default values.
                        if (opInputted == 1)
                        {
                            if (traceOn == 1)
                            {
                                text = strcat(text, "Decision Point: An operation has been inputted\n");
                                decisionCount = decisionCount + 1;
                            }
                            if (!aDefined)
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: 'aDefined' is now true\n");
                                    decisionCount = decisionCount + 1;
                                }
                                aDefined = true;
                            }

                            opDefined = true;
                            increaseDB = false;
                            decimalBalance = 1;

                            if (DEBUG == 1)
                            {
                                cout << "In opInputted if" << endl;
                                cout << "op: " << op << endl;
                                cout << "nextOp: " << nextOp << endl;
                                cout << "aDefined: " << aDefined << endl;
                                cout << "bDefined: " << bDefined << endl;
                                cout << "opDefined: " << opDefined << endl;
                            }

                            // inputs current info to CalcTrace.txt
                            MyFile << "Input: " << op<< endl;
                            MyFile << "op: " << op << endl;
                            MyFile << "ListBox: " << ListBox << endl;
                            MyFile << "a: " << a << endl;
                            MyFile << "b: " << b << endl;
                            MyFile << "\n-----------\n\n";
                        }

                        // if a number button has been selected, the number is appropriately added to the currently selected variable 
                        // (a or b) using the assignInput function (see line 79 for more info) and based on the current state of the system. 
                        if (numInputted == 1)
                        {
                            if ((aDefined == 1) & (bDefined == 0))
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: The input was assigned to 'b'\n");
                                    decisionCount = decisionCount + 1;
                                }

                                assignInput(b, &tempB, input, &increaseDB, decimalBalance);
                                if (increaseDB)
                                {
                                    if (traceOn == 1)
                                    {
                                        text = strcat(text, "Decision Point: The 'decimalBalance' has been increased\n");
                                        decisionCount = decisionCount + 1;
                                    }
                                    decimalBalance = multiply(decimalBalance, 10);
                                }

                                if (negativeB == 1)
                                {
                                    if (traceOn == 1)
                                    {
                                        text = strcat(text, "Decision Point: 'b' was assigned as a negative number\n");
                                        decisionCount = decisionCount + 1;
                                    }
                                    b = -tempB;
                                }
                                else
                                {
                                    if (traceOn == 1)
                                    {
                                        text = strcat(text, "Decision Point: 'b' was assigned as a positive number\n");
                                        decisionCount = decisionCount + 1;
                                    }
                                    b = tempB;
                                }

                                negativeB = false;
                                ListBox = b;
                            }
                            else
                            {
                                if (traceOn == 1)
                                {
                                    text = strcat(text, "Decision Point: The input was assigned to 'a'\n");
                                    decisionCount = decisionCount + 1;
                                }

                                assignInput(a, &tempA, input, &increaseDB, decimalBalance);
                                if (increaseDB)
                                {
                                    if (traceOn == 1)
                                    {
                                        text = strcat(text, "Decision Point: The 'decimalBalance' has been increased\n");
                                        decisionCount = decisionCount + 1;
                                    }
                                    decimalBalance = multiply(decimalBalance, 10);
                                }

                                if (negativeA == 1)
                                {
                                    if (traceOn == 1)
                                    {
                                        text = strcat(text, "Decision Point: 'a' was assigned as a negative number\n");
                                        decisionCount = decisionCount + 1;
                                    }
                                    a = -tempA;
                                }
                                else
                                {
                                    if (traceOn == 1)
                                    {
                                        text = strcat(text, "Decision Point: 'a' was assigned as a positive number\n");
                                        decisionCount = decisionCount + 1;
                                    }
                                    a = tempA;
                                }

                                negativeA = false;
                                ListBox = a;
                            }

                            if (DEBUG == 1)
                            {
                                cout << "In numInputted if" << endl;
                                cout << "aDefined: " << aDefined << endl;
                                cout << "bDefined: " << bDefined << endl;
                                cout << "opDefined: " << opDefined << endl;
                            }

                            // inputs current info to CalcTrace.txt
                            MyFile << "Input: " << input << endl;
                            MyFile << "op: " << op << endl;
                            MyFile << "ListBox: " << ListBox << endl;
                            MyFile << "a: " << a << endl;
                            MyFile << "b: " << b << endl;
                            MyFile << "\n-----------\n\n";
                        }

                        // both boolean variables used to analyze which type of input has been selected are reset to their default values.
                        numInputted = false;
                        opInputted = false;
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Trace Logic"))
                    {   
                        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
                        ImGui::Text("Decision Points: %0f", decisionCount);
                        ImGui::InputTextMultiline("##source", text, 1000, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 25), flags);
                        ImGui::TreePop();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::Separator();
                ImGui::TreePop();
            }
        }

//*********************************************//

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    // Signifies the end of the CalcTrace.txt file and closes the data stream
    MyFile << "EOF\n";
    MyFile.close();

    return 0;
}
