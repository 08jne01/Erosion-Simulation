#include "Program.h"

Program::Program(int width_, int height_) : width(width_), height(height_), eventHandler(camera, GrassGrassSlopeThreshold, GrassBlendAmount)

{
	srand(23);
}

Program::~Program()

{

}

int Program::mainLoop()

{
	if (!glfwInit())

	{
		std::cout << "GLFW did not initialise!" << std::endl;
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Minecraft in c++", NULL, NULL);

	if (!window)

	{
		std::cout << "Failed to create window!" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))

	{
		std::cout << "Failed to initialise GLAD!" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(window, this);
	glfwSwapInterval(1);

	glClearColor(0.3, 0.8, 0.8, 1.0);

	//User Code from Here
	BaseShader vertexShader(GL_VERTEX_SHADER, "Resources/Shaders/BasicVertex.shader");
	BaseShader fragmentShader(GL_FRAGMENT_SHADER, "Resources/Shaders/BasicFrag.shader");
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader.getID());
	glAttachShader(shaderProgram, fragmentShader.getID());
	glLinkProgram(shaderProgram);

	vertexShader.deleteShader();
	fragmentShader.deleteShader();
	
	//world.updateAllChunks();
	//Chunk chunk1;

	//world.addChunk(chunk2);
	//world.genChunk(0, 32);
	
	//chunk2.minusX = &chunk1;
	//chunk1.plusX = &chunk2;

	//chunk.setBlock(0, 1, 0, 1);
	
	

	Clock clock;
	Clock tickrate;
	float updateRate = 1.0 / 60.0;
	float frameTime = 0;

	BufferID bufferIDs;
	Entity entity({0,-100,0}, {0,0,0});
	camera.cameraEntity.position = glm::vec3(-300, -50, -125);
	camera.cameraEntity.rotation = glm::vec3(40, -90, 0);
	Mesh mesh;
	mesh.config = Config(1920, 1080, 0, 1000, 90);
	mesh.shaderProgram = shaderProgram;
	mesh.makeBuffers(bufferIDs);
	mesh.makeHeightMap();
	
	mesh.buildMesh();
	mesh.calcGrad();

	mesh.updateBuffers(bufferIDs, mesh.vertices, mesh.indices);
	
	int droplets = 1000;
	int count = 0;
	while (!glfwWindowShouldClose(window))

	{
		clock.restart();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Update
		camera.update();
		//Draw
		if (count < droplets)

		{
			mesh.errode();
			mesh.buildMesh();
			mesh.calcGrad();
			mesh.updateBuffers(bufferIDs, mesh.vertices, mesh.indices);
			count++;
			if (count >= droplets) std::cout << "Done" << std::endl;
		}

		mesh.setAllUniforms(entity, camera, GrassGrassSlopeThreshold, GrassBlendAmount);
		mesh.draw(bufferIDs, mesh.indices.size());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void Program::framebuffer_size_callback(GLFWwindow* window, int width_, int height_)

{
	glViewport(0, 0, width_, height_);
}

void Program::key_callback(GLFWwindow* window, int button, int scancode, int action, int mods)

{
	if (action == GLFW_RELEASE && button == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);

	void* ptr = glfwGetWindowUserPointer(window);
	Program *kptr = static_cast<Program*>(ptr);

	kptr->eventHandler.keyHandler(action, button);
}

void Program::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)

{
	void* ptr = glfwGetWindowUserPointer(window);
	Program *kptr = static_cast<Program*>(ptr);

	kptr->eventHandler.mouseHandler(xpos, ypos);
}