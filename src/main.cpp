#include "NOU/App.h" 
#include "NOU/Input.h" 
#include "NOU/Entity.h" 
#include "NOU/CCamera.h" 
#include "NOU/CMeshRenderer.h" 
#include "CParticleSystem.h" 
#include "NOU/Shader.h" 
#include "NOU/GLTFLoader.h" 

#include <iostream>

//try making a custom 3d texture class?

//all above inclusions are from NOU
//i am using this in order to create an aesthetically pleasing scene with entities, easier
#include "imgui.h" 
using namespace nou;
void LoadDefaultResources();
//make one shader to use
std::unique_ptr<ShaderProgram> shader, prog_RB, prog_particles;

int shaderToggleInfo = 0;//the data to tell the shader what to toggle 
bool isPowerful = false;
unsigned int textureColorbuffer;
unsigned int framebuffer;
unsigned int rbo;
// Templated LERP function 
template<typename T>
T Lerp(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}
std::vector<Entity*> renderingEntities;

glm::vec2 screenRes;
int main() {
	App::Init("CG Exam - 100782747", 1280, 720);
	App::SetClearColor(glm::vec4(0.454, 0.854, 0.364,1));
	// Initialize ImGui 
	App::InitImgui();
	screenRes = glm::vec2(1280, 720);
	// Load in our model/texture resources 
	LoadDefaultResources();


	// Create and set up camera 
	Entity cameraEntity = Entity::Create();
	CCamera& cam = cameraEntity.Add<CCamera>(cameraEntity);
	cam.Perspective(60.0f, (float)1280 / 720, 0.001f, 100.0f);
	//cam.Perspective(100.f, (float) width/height, 0.1f, 100.0f); 
	cameraEntity.transform.m_pos = glm::vec3(-0.7f, -1.2f, -10.7f);
	cameraEntity.transform.m_rotation = glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
	//cameraEntity.transform.m_rotation = glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f));





	std::unique_ptr<Material> confettiParticle;
	std::unique_ptr<Mesh> particleMesh = std::make_unique<Mesh>();

	ParticleParam newParticle;
	newParticle.lifetime = 5.f;
	newParticle.startColor = glm::vec4(1, 0, 0, 1.f);
	newParticle.endColor = glm::vec4(1, 0, 0, 0.f);
	newParticle.startSize = 0.1f;
	newParticle.maxParticles = 100;
	newParticle.emissionRate = 10.f;
	newParticle.tanTheta = glm::tan(glm::radians(45.0f));

	GLTF::LoadMesh("tile.gltf", *particleMesh);
	std::unique_ptr<Texture2D> particleTexture = std::make_unique<Texture2D>("particle.png");
	std::unique_ptr<ShaderProgram> particleShader = std::make_unique<ShaderProgram>(*prog_particles);
	std::unique_ptr<Material> particleMat = std::make_unique<Material>(*particleShader);
	particleMat->AddTexture("albedo", *particleTexture);

	Entity confettiEmitter = Entity::Create();
	confettiEmitter.transform.m_pos = glm::vec3(-0.526, -1.009, -14.464);
	confettiEmitter.Add<CParticleSystem>(confettiEmitter, *particleMat, newParticle);
	//confettiEmitter.Get<CParticleSystem>().setYSpeed(1);
	renderingEntities.push_back(&confettiEmitter);

	std::unique_ptr<Mesh> pacMesh = std::make_unique<Mesh>();

	GLTF::LoadMesh("shape.gltf", *pacMesh);
	std::unique_ptr<Texture2D> pacTexture = std::make_unique<Texture2D>("pac.png");
	std::unique_ptr<ShaderProgram> pacShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> pacMat = std::make_unique<Material>(*pacShader);

	pacMat->AddTexture("albedo", *pacTexture);




	Entity pac = Entity::Create();
	pac.Add<CMeshRenderer>(pac, *pacMesh, *pacMat);
	pac.transform.m_scale = glm::vec3(0.266);
	pac.transform.m_rotation = glm::angleAxis(glm::radians(00.f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::angleAxis(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
	pac.transform.m_pos = glm::vec3(-0.526, -1.599, -14.464);
	//pac.transform.m_pos = glm::vec3(-2.760, -1.599, -12.464);
	renderingEntities.push_back(&pac);
	//chick.transform.RecomputeGlobal();



	std::unique_ptr<Mesh> pelletMesh = std::make_unique<Mesh>();

	GLTF::LoadMesh("shape.gltf", *pelletMesh);
	std::unique_ptr<Texture2D> pelletTexture = std::make_unique<Texture2D>("pellet.png");
	std::unique_ptr<ShaderProgram> pelletShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> pelletMat = std::make_unique<Material>(*pelletShader);

	pelletMat->AddTexture("albedo", *pelletTexture);


	Entity pellet = Entity::Create();
	pellet.Add<CMeshRenderer>(pellet, *pelletMesh, *pelletMat);
	pellet.transform.m_scale = glm::vec3(0.166);
	pellet.transform.m_rotation = glm::angleAxis(glm::radians(00.f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::angleAxis(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
	pellet.transform.m_pos = glm::vec3(-0.826, -2.309, -14.464);
	//pellet.transform.m_pos = glm::vec3(-2.760, -1.599, -12.464);
	renderingEntities.push_back(&pellet);


	std::unique_ptr<Mesh> ghostMesh = std::make_unique<Mesh>();

	GLTF::LoadMesh("shape.gltf", *ghostMesh);
	std::unique_ptr<Texture2D> ghostTexture = std::make_unique<Texture2D>("ghost.png");
	std::unique_ptr<ShaderProgram> ghostShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> ghostMat = std::make_unique<Material>(*ghostShader);

	ghostMat->AddTexture("albedo", *ghostTexture);


	Entity ghost = Entity::Create();
	ghost.Add<CMeshRenderer>(ghost, *ghostMesh, *ghostMat);
	ghost.transform.m_scale = glm::vec3(0.266,1.0,0.266);
	ghost.transform.m_rotation = glm::angleAxis(glm::radians(00.f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::angleAxis(glm::radians(00.f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
	ghost.transform.m_pos = glm::vec3(-0.526, -0.509, -14.464);
	//ghost.transform.m_pos = glm::vec3(-2.760, -1.599, -12.464);
	renderingEntities.push_back(&ghost);

	std::unique_ptr<Mesh> superPelletMesh = std::make_unique<Mesh>();

	GLTF::LoadMesh("shape.gltf", *superPelletMesh);
	std::unique_ptr<Texture2D> superPelletTexture = std::make_unique<Texture2D>("superPellet.png");
	std::unique_ptr<ShaderProgram> superPelletShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> superPelletMat = std::make_unique<Material>(*superPelletShader);

	superPelletMat->AddTexture("albedo", *superPelletTexture);


	Entity superPellet = Entity::Create();
	superPellet.Add<CMeshRenderer>(superPellet, *superPelletMesh, *superPelletMat);
	superPellet.transform.m_scale = glm::vec3(0.166);
	superPellet.transform.m_rotation = glm::angleAxis(glm::radians(00.f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::angleAxis(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
	superPellet.transform.m_pos = glm::vec3(-0.526, -0.109, -14.464);
	//superPellet.transform.m_pos = glm::vec3(-2.760, -1.599, -12.464);
	renderingEntities.push_back(&superPellet);
	
	std::unique_ptr<Mesh> winMesh = std::make_unique<Mesh>();

	GLTF::LoadMesh("tile.gltf", *winMesh);
	std::unique_ptr<Texture2D> winTexture = std::make_unique<Texture2D>("win.png");
	std::unique_ptr<ShaderProgram> winShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> winMat = std::make_unique<Material>(*winShader);

	winMat->AddTexture("albedo", *winTexture);

	std::unique_ptr<Mesh> loseMesh = std::make_unique<Mesh>();
	GLTF::LoadMesh("tile.gltf", *loseMesh);
	std::unique_ptr<Texture2D> loseTexture = std::make_unique<Texture2D>("lose.png");
	std::unique_ptr<ShaderProgram> loseShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> loseMat = std::make_unique<Material>(*loseShader);

	loseMat->AddTexture("albedo", *loseTexture);


	Entity tile = Entity::Create();
	tile.Add<CMeshRenderer>(tile, *winMesh, *winMat);
	tile.transform.m_scale = glm::vec3(1.3);
	tile.transform.m_rotation = glm::angleAxis(glm::radians(00.f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::angleAxis(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
	tile.transform.m_pos = glm::vec3(-0.526, -0.309, -14.464);
	//tile.transform.m_pos = glm::vec3(-2.760, -1.599, -12.464);
	//renderingEntities.push_back(&tile);
	/*
	Entity background = Entity::Create();
	background.Add<CMeshRenderer>(background, *backgroundMesh, *backgroundMat);
	background.transform.m_scale = glm::vec3(0.469);
	background.transform.m_rotation = glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
	background.transform.m_pos = glm::vec3(-0.700, -2.239, -12.895);
	renderingEntities.push_back(&background);
	//background.transform.RecomputeGlobal();
	*/

	//glUniform1i(shader->GetUniformLoc("LUT"), 0);
	//glUniform1i(shader->GetUniformLoc("LUT2"), 1);
	GLuint albedoUniform;
	GLuint quadVAO;
	albedoUniform = prog_RB->GetUniformLoc("albedo");
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// generate texture

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenRes.x, screenRes.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, res.x, res.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);


	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenRes.x, screenRes.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	float quadVertices[] = {
		// positions  
		-1.0f,  1.0f,0.f,
		-1.0f, -1.0f,0.f,
		 1.0f, -1.0f ,0.f,

		-1.0f,  1.0f,0.f,
		 1.0f, -1.0f,0.f,
		 1.0f,  1.0f,0.f
	};
	float quadUV[] = { 0,  1.0f ,0, 0, 1.0f, 0,
	0,  1.0f, 1.0f, 0,1.0f,  1.0f };

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	GLuint pos;
	GLuint uv;
	glGenBuffers(1, &pos);
	glBindBuffer(GL_ARRAY_BUFFER, pos);
	glBufferData(GL_ARRAY_BUFFER, std::size(quadVertices) * sizeof(float), &quadVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);//uv

	//std::cout << std::size(quadUV) << std::endl;
	glGenBuffers(1, &uv);
	glBindBuffer(GL_ARRAY_BUFFER, uv);
	glBufferData(GL_ARRAY_BUFFER, std::size(quadUV) * sizeof(float), &quadUV[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);//uv






	App::Tick();
	float scale = 0;

	float t = 0;
	float a = 0, b = 0, c = 0;
	float stren = 0.f;
	int mode = 0;
	int mode2 = 0;
	float speed = 3;
	float daySeconds = 5;
	float currentDaySeconds = 0;
	float tMulti = 1;
	float dayMulti = 1;
	bool hasWon = false;
	glm::vec3 dayLight = glm::vec3(0.95);
	glm::vec3 nightLight = glm::vec3(0.25);

	glm::vec3 spinVector = glm::vec3(90, 45, 75);
	//shader.get()->GetUniformLoc("LUT");//calls glGetUniformLocation in shader

	float totalGameSeconds = 0.f;
	
	bool isJumping = false;
	float lowY = -2;
	float highY = 0;
	float jumpI = 0.f;
	float jumpTotal = 0.f;
	bool hasPlayerShot = false;
	bool hasSnakeShot = false;
	bool shouldAmbient = false;
	bool shouldDiffuse = false;
	bool shouldSpecular = false;

	glm::vec3 playerShotOrigin;
	glm::vec3 playerShotDestination = glm::vec3(-999);
	glm::vec3 snakeShotOrigin;
	glm::vec3 snakeShotDestination = glm::vec3(-999);
	float snakeShotT = 0;
	float playerShotT = 0;
	superPellet.transform.m_pos = glm::vec3(-999);
	pellet.transform.m_pos = glm::vec3(-999);
	int snakeHits = 0;

	while (!App::IsClosing() && (!Input::GetKeyDown(GLFW_KEY_BACKSPACE) && !Input::GetKeyDown(GLFW_KEY_ESCAPE)))
	{
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();
		totalGameSeconds += deltaTime;

		int floorGameSec = (int)floor(totalGameSeconds);
		if (floorGameSec % 3 == 2 && snakeHits < 3) {
			hasSnakeShot = true;
			
			
			
		}
		if (hasSnakeShot) {
			if (snakeShotDestination.x < -998) {
				snakeShotOrigin = ghost.transform.m_pos + glm::vec3(0, 1, 0);
				snakeShotDestination = pac.transform.m_pos;
			}
			snakeShotT += deltaTime;
			superPellet.transform.m_pos = (Lerp(snakeShotOrigin, snakeShotDestination, snakeShotT));
			if (snakeShotT > 1) {
				hasSnakeShot = false;
				snakeShotDestination = glm::vec3(-999);
				superPellet.transform.m_pos = glm::vec3(-999);
				snakeShotT = 0;
			}
		}
		if (hasPlayerShot) {
			if (playerShotDestination.x < -998) {
				playerShotOrigin = pac.transform.m_pos;
				
				playerShotDestination = playerShotOrigin + glm::vec3(10, 0, 0);
			}
			playerShotT += deltaTime;
			pellet.transform.m_pos = Lerp(playerShotOrigin, playerShotDestination, playerShotT);
			if (playerShotT > 1) {
				playerShotT = 0;
				hasPlayerShot = false;
				playerShotDestination = glm::vec3(-999);
			}
		}
		cameraEntity.Get<CCamera>().Update();
		cameraEntity.transform.RecomputeGlobal();

		
		if (isJumping) {
			jumpI += deltaTime;
			if (jumpI < 0.5) {
				jumpTotal += deltaTime * 2;
			}
			else
			{
				jumpTotal -= deltaTime * 2;
			}
			if (jumpI > 1) {
				jumpI = 0;
				isJumping = false;
				jumpTotal = 0;

			}
			//pac.transform.m_pos.y = Lerp(lowY, highY, jumpTotal);
			
		}
		pac.transform.m_pos.y = Lerp(lowY, highY, jumpTotal);
		t += deltaTime / 1 * tMulti;
		if (t > 1 || t < 0) {
			tMulti *= -1;
		}

		currentDaySeconds += deltaTime / daySeconds * dayMulti;
		if (currentDaySeconds > 1 || currentDaySeconds < 0) {
			dayMulti *= -1;
		}
		//ShaderProgram::Current()->SetUniform("LUT", transform.GetNormal());

		//glBindTextureUnit(LUTTexture.get()->GetID(), shader.get()->GetUniformLoc("LUT"));

		shader.get()->Bind();
		shader.get()->SetUniform("mode", mode);
		shader.get()->SetUniform("shouldAmbient", (int)shouldAmbient);
		shader.get()->SetUniform("shouldDiffuse", (int)shouldDiffuse);
		shader.get()->SetUniform("shouldSpecular", (int)shouldSpecular);
		
		shader.get()->SetUniform("mode2", mode2);
		shader.get()->SetUniform("lightColor", Lerp(dayLight, nightLight, currentDaySeconds));

		prog_RB->Bind();
		prog_RB.get()->SetUniform("shouldBuffer", (int)true);
		prog_RB.get()->SetUniform("passedTime", totalGameSeconds);
		prog_RB.get()->SetUniform("filmGrainStrength", 16.5f);
		//prog_RB.get()->SetUniform("blurStr", (int)floor(Lerp(blurBounds.x, blurBounds.y, blurT)));
		prog_RB.get()->SetUniform("screenRes", screenRes);

	

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.454, 0.854, 0.364,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now

	for each (Entity * e in renderingEntities) {
		e->transform.RecomputeGlobal();
		if (e->Has<CMeshRenderer>()) {
			
			e->Get<CMeshRenderer>().Draw();
		}
	}

	

	// second pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(0.454, 0.854, 0.364,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_CW);
	prog_RB->Bind();



	glDisable(GL_DEPTH_TEST);
	//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	glUniform1i(albedoUniform, 0);
	glActiveTexture(GL_TEXTURE0 + 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

	glBindVertexArray(quadVAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glViewport(0, 0, resoloutions[accessSettings[8]].x, resoloutions[accessSettings[8]].y);

//App::SwapBuffers();
//glDeleteFramebuffers(1, &framebuffer);

	glEnable(GL_DEPTH_TEST);
		
	
		confettiEmitter.transform.RecomputeGlobal();

		confettiEmitter.Get<CParticleSystem>().Update(deltaTime, hasWon, true);

		glDisable(GL_DEPTH_TEST);
		confettiEmitter.Get<CParticleSystem>().Draw();
		glEnable(GL_DEPTH_TEST);
		
		glDisable(GL_DEPTH_TEST);




		//	glViewport(0, 0, resoloutions[accessSettings[8]].x, resoloutions[accessSettings[8]].y);
		App::SwapBuffers();
		//glDeleteFramebuffers(1, &framebuffer);

		glEnable(GL_DEPTH_TEST);
		/*
		App::StartImgui();
		ImGui::SetNextWindowPos(ImVec2(0, 800), ImGuiCond_FirstUseEver);
		ImGui::DragFloat("X", &(a), 0.1f);
		ImGui::DragFloat("Y", &(b), 0.1f);
		ImGui::DragFloat("Z", &(c), 0.1f);
		ImGui::DragFloat("S", &(scale), 0.1f);
		//ImGui::DragInt("M", &(mode), 0.01);
		App::EndImgui();
		//chick.transform.m_pos = Lerp(glm::vec3(1.450, -1.599, -12.464), glm::vec3(-2.760, -1.599, -12.464), t);
		*/
		glm::vec2 vel = glm::vec2(0, 0);
		if (Input::GetKey(GLFW_KEY_SPACE)) {
			isJumping = true;
		}
		if (Input::GetKey(GLFW_KEY_E)) {
			hasPlayerShot = true;
		}
		if (Input::GetKey(GLFW_KEY_A)) {
			vel += glm::vec2(-1, 0);
		}
		if (Input::GetKey(GLFW_KEY_D)) {
			vel += glm::vec2(1, 0);
		}

		pac.transform.m_pos.x += vel.x * speed * deltaTime;
		pac.transform.m_pos.y += vel.y * speed * deltaTime;



		//pellet.transform.m_rotation.x += speed * deltaTime;
		//superPellet.transform.m_rotation.x -= speed * deltaTime;
		superPellet.transform.m_rotation = glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::angleAxis(glm::radians(90.f + Lerp(0, 360, t)), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));

		ghost.transform.m_pos = Lerp(glm::vec3(1.450, -0.599, -14.464), glm::vec3(1.950, -0.599, -14.464), t);
		if (glm::distance(pac.transform.m_pos, ghost.transform.m_pos) < 0.5) {
			if (!isPowerful) {
				tile.Remove<CMeshRenderer>();
				tile.Add<CMeshRenderer>(tile, *loseMesh, *loseMat);

				renderingEntities.push_back(&tile);
			}
			else
			{
				renderingEntities.push_back(&tile);
				hasWon = true;
				
			}
			renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &ghost), renderingEntities.end());
			renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &pac), renderingEntities.end());
			renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &superPellet), renderingEntities.end());
			renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &pellet), renderingEntities.end());

		}

		if (glm::distance(pac.transform.m_pos, superPellet.transform.m_pos) < 0.25) {
			//isPowerful = true;
			//renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &superPellet), renderingEntities.end());
			tile.Remove<CMeshRenderer>();
			tile.Add<CMeshRenderer>(tile, *loseMesh, *loseMat);
			renderingEntities.push_back(&tile);
			renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &ghost), renderingEntities.end());
			renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &pac), renderingEntities.end());
			renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &superPellet), renderingEntities.end());
			renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &pellet), renderingEntities.end());

		}

		if (glm::distance(ghost.transform.m_pos, pellet.transform.m_pos) < 1) {
			snakeHits++;
			playerShotT = 0;
			hasPlayerShot = false;
			playerShotDestination = glm::vec3(-999);
			pellet.transform.m_pos = glm::vec3(-999);
			if (snakeHits > 2) {
				renderingEntities.push_back(&tile);
				renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &ghost), renderingEntities.end());
				renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &pac), renderingEntities.end());
				renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &superPellet), renderingEntities.end());
				renderingEntities.erase(std::remove(renderingEntities.begin(), renderingEntities.end(), &pellet), renderingEntities.end());
				hasWon = true;
			}
			
		}

		if (Input::GetKeyDown(GLFW_KEY_1)) {
			mode = 1;
		}
		if (Input::GetKeyDown(GLFW_KEY_2)) {
			shouldAmbient = !shouldAmbient;
		}
		if (Input::GetKeyDown(GLFW_KEY_3)) {
			shouldDiffuse = !shouldDiffuse;
		}
		if (Input::GetKeyDown(GLFW_KEY_4)) {
			shouldSpecular = !shouldSpecular;
		}
		if (Input::GetKeyDown(GLFW_KEY_0)) {
			mode = 0;
		}
		if (Input::GetKeyDown(GLFW_KEY_6)) {
			mode = 0;
			shouldAmbient = false;
			shouldDiffuse = false;
			shouldSpecular = false;
			mode2 = 0;
		}
		if (Input::GetKeyDown(GLFW_KEY_5)) {
			if (mode2 == 1) {
				mode2 = 0;
			}
			else
			{
				mode2 = 1;
			}

		}
		//std::cout << glm::distance(pac.transform.m_pos, ghost.transform.m_pos) << std::endl;
		//App::SwapBuffers();
	}
	App::Cleanup();

	return 0;
}

void LoadDefaultResources() {
	// Load in the shaders we will need and activate them 
	// Textured lit shader 
	std::unique_ptr vs_Shader = std::make_unique<Shader>("shaderVS.vert", GL_VERTEX_SHADER);
	//std::unique_ptr fs_texLitShader = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);
	std::unique_ptr fs_Shader = std::make_unique<Shader>("shaderFS.frag", GL_FRAGMENT_SHADER);
	std::vector<Shader*> texLit = { vs_Shader.get(), fs_Shader.get() };
	shader = std::make_unique<ShaderProgram>(texLit);

	
		try
		{
			std::unique_ptr vs_RB = std::make_unique<Shader>("RB.vert", GL_VERTEX_SHADER);
			//std::unique_ptr fs_texLitShader = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);
			std::unique_ptr fs_RB = std::make_unique<Shader>("RB.frag", GL_FRAGMENT_SHADER);
			std::vector<Shader*> RB = { vs_RB.get(), fs_RB.get() };
			prog_RB = std::make_unique<ShaderProgram>(RB);
		}
		catch (const std::exception& e) {
			std::cout << "RENDERBUFFER ERROR: " << e.what() << std::endl;
		}

	
		//Billboarded particles shader program. 
		auto v_particles = std::make_unique<Shader>("particles.vert", GL_VERTEX_SHADER);
		auto g_particles = std::make_unique<Shader>("particles.geom", GL_GEOMETRY_SHADER);
		auto f_particles = std::make_unique<Shader>("particles.frag", GL_FRAGMENT_SHADER);

		try
		{
			std::vector<Shader*> particles = { v_particles.get(), g_particles.get(), f_particles.get() };
			prog_particles = std::make_unique<ShaderProgram>(particles);
		}
		catch (const std::exception& e) {
			std::cout << "PARTICLE SHADER ERROR: " << e.what() << std::endl;
		}
}
