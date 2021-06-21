#include "Outline_and_ForceField_Example.h"

Outline_and_ForceField_Example::Outline_and_ForceField_Example()
{
    

    
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    depthMapTex = Texture(shadowWidth, shadowHeight, GL_DEPTH_COMPONENT, GL_REPEAT, GL_NEAREST, GL_FLOAT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTex.getTextureBinding(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    colorBufferTex = Texture(width, height, GL_RGB, GL_CLAMP, GL_LINEAR, GL_UNSIGNED_BYTE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferTex.getTextureBinding(), 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glGenFramebuffers(1, &customColorBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, customColorBuffer);
    customBufferTex = Texture(width, height, GL_RGB, GL_CLAMP, GL_LINEAR, GL_UNSIGNED_BYTE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, customBufferTex.getTextureBinding(), 0);
   
    depthShader.loadShaderFromFile("shaders/AdvancedLightning/ShadowProjV.glsl", GL_VERTEX_SHADER);
    depthShader.loadShaderFromFile("shaders/AdvancedLightning/DepthShaderFrag.glsl", GL_FRAGMENT_SHADER);
    depthShader.attachAndCompile();

   
    testPlainShader.loadShaderFromFile("shaders/VertexScreen.glsl", GL_VERTEX_SHADER);
    testPlainShader.loadShaderFromFile("shaders/AdvancedLightning/outlinePP.frag", GL_FRAGMENT_SHADER);
    // testPlainShader.loadShaderFromFile("shaders/TextureShader.glsl", GL_FRAGMENT_SHADER);
    testPlainShader.attachAndCompile();


   
    portalShader.loadShaderFromFile("shaders/AdvancedLightning/portal.vert", GL_VERTEX_SHADER);
    portalShader.loadShaderFromFile("shaders/AdvancedLightning/portal.frag", GL_FRAGMENT_SHADER);
    portalShader.attachAndCompile();

   
    unlitShader.loadShaderFromFile("shaders/AdvancedLightning/ShadowProjV.glsl", GL_VERTEX_SHADER);
    unlitShader.loadShaderFromFile("shaders/AdvancedLightning/unlit.frag", GL_FRAGMENT_SHADER);
    unlitShader.attachAndCompile();

    shaderProg.loadShaderFromFile("shaders/AdvancedLightning/shadowmapper.vert", GL_VERTEX_SHADER);
    shaderProg.loadShaderFromFile("shaders/AdvancedLightning/shadowmapper.frag", GL_FRAGMENT_SHADER);
    shaderProg.attachAndCompile();

    shaderLightProg.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
    // shaderLightProg.loadShaderFromFile("shaders/AdvancedLightning/outlinePP.frag", GL_FRAGMENT_SHADER);
     //shaderLightProg.loadShaderFromFile("shaders/LightShader.glsl", GL_FRAGMENT_SHADER);
    shaderLightProg.attachAndCompile();
    cubeM = Mesh(CubeMeshPreset::vertecies, CubeMeshPreset::normals, CubeMeshPreset::uvs);
    //   Mesh lightCube = Mesh(CubeMeshPreset::vertecies, CubeMeshPreset::normals, CubeMeshPreset::uvs);   
    mainT = Texture("texture.jpg");
    noiseT = Texture("2.jpg");
    float dt = 0;
    modelPortal = glm::translate(modelPortal, glm::vec3(-1.5f, 1.4f, 1.4f));

}

void Outline_and_ForceField_Example::renderLoop(Camera& mainCamera)
{
    double now = glfwGetTime();
    lightCubesPositions[0] = mainCamera.cameraPos;
    glm::vec3 lightDir = glm::vec3(0, 0, 0);
    float near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(lightCubesPositions[0], mainCamera.cameraPos + mainCamera.cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = mainCamera.projection * lightView;




    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, shadowWidth, shadowHeight);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < 4; i++)
    {
        depthShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubesPositions[i]);
        depthShader.setUniformMat4("model", model);
        depthShader.setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
        cubeM.Draw(depthShader);
    }

    //pass 2
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (size_t i = 0; i < 4; i++)
    {
        shaderProg.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubesPositions[i]);
        shaderProg.setSampler("diffuseTexture", 0, mainT);
        shaderProg.setSampler("shadowMap", 1, depthMapTex);
        shaderProg.setObjectMPV(model, mainCamera.getView(), mainCamera.projection);
        shaderProg.setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
        shaderProg.setUniformV3("viewPos", mainCamera.cameraPos);
        shaderProg.setUniformV3("lightPos", lightCubesPositions[0]);      
        cubeM.Draw(shaderProg);
    }
    portalShader.use();
    portalShader.setSampler("shadowMap", 0, depthMapTex);
    portalShader.setObjectMPV(modelPortal, mainCamera.getView(), mainCamera.projection);
    portalShader.setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
    portalShader.setUniformMat4("cameraModel", glm::translate(glm::mat4(1.0f), mainCamera.cameraPos));
    shaderProg.setUniformV3("viewPos", mainCamera.cameraPos);
    shaderProg.setUniformV3("lightPos", lightCubesPositions[0]);
    sphere.Draw(portalShader);



    glBindFramebuffer(GL_FRAMEBUFFER, customColorBuffer);
    glClearColor(0.0, 0.0, 0.0, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (size_t i = 0; i < 4; i++)
    {
        unlitShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubesPositions[i]);
        unlitShader.setObjectMPV(model, mainCamera.getView(), mainCamera.projection);
        unlitShader.setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
        cubeM.Draw(unlitShader);
    }



    //PostProcess     
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    testPlainShader.use();
    testPlainShader.setSampler("mainTexture", 0, colorBufferTex);
    testPlainShader.setSampler("selectionObjTexture", 1, customBufferTex);
    quad.Draw(testPlainShader);


}
