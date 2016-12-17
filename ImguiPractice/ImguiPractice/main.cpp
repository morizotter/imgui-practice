//
//  main.cpp
//  ImguiPractice
//
//  Created by MORITANAOKI on 2016/12/02.
//  Copyright © 2016年 molabo. All rights reserved.
//

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

using namespace std;
using namespace cv;

string win_name = "Window";
string video_name = "video1.mp4";
const char* vertex_file_name = "simple.vert";
const char* fragment_file_name = "simple.frag";

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include "shader.hpp"

int main( void ) {
    VideoCapture capture(video_name);
    if (!capture.isOpened())
    {
        cerr << "Failed to open the video device, video file or image sequence!\n" << endl;
        return 0;
    }
    
    int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    printf("video size %d %d\n", width, height);
    
    if( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(width, height, "ImGui", NULL, NULL);
    if( window == NULL )
    {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    ImGui_ImplGlfwGL3_Init(window, true);
    bool show_another_window = false;
    bool show_test_window = true;
    float slider_f = 0.0;
    
    // setup opengl
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    GLuint programID = LoadShaders(vertex_file_name, fragment_file_name);
    
    GLuint Texture;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_RECTANGLE, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    GLuint TextureID  = glGetUniformLocation(programID, "textureSampler");
    
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
    };
    
    // Two UV coordinatesfor each vertex.
    static const GLfloat g_uv_buffer_data[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    
    do{
        Mat frame;
        
        capture >> frame;
        if (frame.empty()) {  // auto rewind -- movie file playback only
            capture.set(CV_CAP_PROP_POS_FRAMES, 0);
            capture >> frame;
        }
        
        glBindTexture(GL_TEXTURE_RECTANGLE, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(programID);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(TextureID, 0);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2,GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 4 indices starting at 0 -> 1 rectangle
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        ImGui_ImplGlfwGL3_NewFrame();
        
        {
            ImGui::Begin("First Window");
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &slider_f, 0.0f, 1.0f);
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }
        
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }
        
        ImGui::Render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
    
    ImGui_ImplGlfwGL3_Shutdown();
    
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &TextureID);
    glDeleteVertexArrays(1, &VertexArrayID);
    
    glfwTerminate();
    
    return 0;
}
