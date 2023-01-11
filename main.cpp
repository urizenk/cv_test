#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);




const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;




//暂时硬编码顶点着色器

const char * vertexShaderSource = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "void main()\n"
                                  "{\n"
                                  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                  "}\0";

//片段着色器
const char * fragmentShaderSource = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "\n"
                                    "void main()\n"
                                    "{\n"
                                    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                    "}\n\0 ";

int main() {

    //初始化设置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);



    //创建窗口对象
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"first",NULL,NULL);
    if (window == NULL){
        cout << "创建glfw窗口失败" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    //注册回调
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    //初始化glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "初始化glad失败";
        return -1;
    }

//    //定义视口
//    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);

    /**
     * 创建着色器
     */

    //创建顶点着色器
    //顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //附加源码到着色器上，然后编译
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    //检查编译是否成功
    int success;
    char logInfo[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL,logInfo);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\\n" << logInfo << endl;
    }

    //创建片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,logInfo);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\\n" << logInfo << endl;
    }


    //创建着色器程序
    unsigned int shaderProgram = glCreateProgram();


    //将着色器附加到程序上并连接
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(shaderProgram,512,NULL,logInfo);
        cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\\n" << logInfo << endl;
    }
//    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
//    if(!success)


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /**
     * 创建缓冲区
     */

    //顶点坐标
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    unsigned int VBO,VAO;

    //生成VAO
    glGenVertexArrays(1,&VAO);
    //生成VBO缓冲对象
    glGenBuffers(1,&VBO);
    //绑定属性数组
    glBindVertexArray(VAO);
    //绑定缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //把顶点数据复制到缓冲中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //设置顶点指针
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3 * sizeof(float ), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES,0,3);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);



    //实现循环渲染
    while(!glfwWindowShouldClose(window)){

        //检查输入
        processInput(window);

        //TODO 渲染指令
        glClearColor(0.2f,0.3f,0.15f,0.8f);
        glClear(GL_COLOR_BUFFER_BIT);

        //画三角型
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);


        //检查调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 生命周期结束就关闭
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteProgram(shaderProgram);
    //关闭窗口
    glfwTerminate();

    return 0;


}

//视口改变的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}

//处理输入
void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}






