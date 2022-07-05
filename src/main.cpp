#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Context.h"


// Define targa header. This is only used locally.
#pragma pack(1)
typedef struct
{
    GLbyte	identsize;              // Size of ID field that follows header (0)
    GLbyte	colorMapType;           // 0 = None, 1 = paletted
    GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short	colorMapStart;          // First colour map entry
    unsigned short	colorMapLength;         // Number of colors
    unsigned char 	colorMapBits;   // bits per palette entry
    unsigned short	xstart;                 // image x origin
    unsigned short	ystart;                 // image y origin
    unsigned short	width;                  // width in pixels
    unsigned short	height;                 // height in pixels
    GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte	descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)

// Capture the current viewport and save it as a targa file.
// Be sure and call SwapBuffers for double buffered contexts or
// glFinish for single buffered contexts before calling this function.
// Returns 0 if an error occurs, or 1 on success.
GLint gltWriteTGA(const char *szFileName)
{
    FILE *pFile;                // File pointer
    TGAHEADER tgaHeader;		// TGA file header
    unsigned long lImageSize;   // Size in bytes of image
    GLbyte	*pBits = NULL;      // Pointer to bits
    GLint iViewport[4];         // Viewport in pixels
    GLenum lastBuffer;          // Storage for the current read buffer setting

    // Get the viewport dimensions
    glGetIntegerv(GL_VIEWPORT, iViewport);

    // How big is the image going to be (targas are tightly packed)
    lImageSize = iViewport[2] * 3 * iViewport[3];

    // Allocate block. If this doesn't work, go home
    pBits = (GLbyte *)malloc(lImageSize);
    if (pBits == NULL)
        return 0;

    // Read bits from color buffer
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

    // Get the current read buffer setting and save it. Switch to
    // the front buffer and do the read operation. Finally, restore
    // the read buffer state
    glGetIntegerv(GL_READ_BUFFER, (GLint *)&lastBuffer);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, iViewport[2], iViewport[3], GL_BGR, GL_UNSIGNED_BYTE, pBits);
    glReadBuffer(lastBuffer);

    // Initialize the Targa header
    tgaHeader.identsize = 0;
    tgaHeader.colorMapType = 0;
    tgaHeader.imageType = 2;
    tgaHeader.colorMapStart = 0;
    tgaHeader.colorMapLength = 0;
    tgaHeader.colorMapBits = 0;
    tgaHeader.xstart = 0;
    tgaHeader.ystart = 0;
    tgaHeader.width = iViewport[2];
    tgaHeader.height = iViewport[3];
    tgaHeader.bits = 24;
    tgaHeader.descriptor = 0;

    // Attempt to open the file
    pFile = fopen(szFileName, "wb");
    if (pFile == NULL)
    {
        free(pBits);    // Free buffer and return error
        return 0;
    }

    // Write the header
    fwrite(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

    // Write the image data
    fwrite(pBits, lImageSize, 1, pFile);

    // Free temporary buffer and close the file
    free(pBits);
    fclose(pFile);

    // Success!
    return 1;
}



void OnFramebufferSizeChange(GLFWwindow *window, int width, int height) {
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    auto context = reinterpret_cast<Context *>(glfwGetWindowUserPointer(window));
    context->Reshape(width, height);
}

void OnKeyEvent(GLFWwindow *window,
                int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void OnCursorPos(GLFWwindow *window, double x, double y) {
    auto context = (Context *) glfwGetWindowUserPointer(window);
    context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int modifier) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);
    auto context = (Context*)glfwGetWindowUserPointer(window);
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    context->MouseButton(button, action, x, y);
}

void OnCharEvent(GLFWwindow* window, unsigned int ch) {
    ImGui_ImplGlfw_CharCallback(window, ch);
}

void OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

int main(int argc, const char **argv) {
//    Logger::Init();
//    CHA_CORE_WARN("Start program");
    // Logging start of program
    SPDLOG_INFO("Start program");

    // Initializing glfw library
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) {
        const char *description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core Profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow Forward Compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    // Create GLFW window
    SPDLOG_INFO("Create glfw window");
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // OpenGL function loading with glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        SPDLOG_ERROR("failed to initialize glad");
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // OpenGL Version check
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", glVersion);

    auto imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();

    auto context = Context::Create();
    if (!context) {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }
    glfwSetWindowUserPointer(window, context.get());

    // Define Callback function
#ifdef __APPLE__
    OnFramebufferSizeChange(window, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);
#else
    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
#endif
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);
    glfwSetCharCallback(window, OnCharEvent);
    glfwSetCursorPosCallback(window, OnCursorPos);
    glfwSetMouseButtonCallback(window, OnMouseButton);
    glfwSetScrollCallback(window, OnScroll);

    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");

    double lastTime = glfwGetTime(); //#11
    int numOfFrames = 0;
    int count = 0;
//    int frameNum = 1; &&  frameNum++ <= 300
    while (!glfwWindowShouldClose(window) ) {
        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        context->ProcessInput(window);
        context->Render();

        double currentTime = glfwGetTime();
        numOfFrames++;


        if (currentTime - lastTime >= 1.0) {

            printf("%f ms/frame  %d fps \n", 1000.0 / double(numOfFrames), numOfFrames);
            numOfFrames = 0;
            lastTime = currentTime;
        }
        // save each frame as a targa file
//        if (!(frameNum % 20)) {
//            std::string fileName = std::to_string(frameNum) + "_frame.tga";
//            const char * c = fileName.c_str();
//            if(gltWriteTGA(c))
//                printf("saving %d sucess!\n", frameNum);
//        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    context.reset();

    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiContext);

    glfwTerminate();
    return 0;
}
