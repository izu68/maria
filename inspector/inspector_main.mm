#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

extern "C"
{
	int init_inspector_window ( void );
	void update_inspector_window ( void );
	void destroy_inspector_window ( void );
	bool inspector_sent_reset ( void );
	void emu_done_reset ( void );
	void inspector_transfer_eva_registers ( int r0, int r1, int r2, int r3, int flags, int pc, int addr_bank, int addr, int data );
}

static void glfw_error_callback ( int error, const char* description )
{
	fprintf ( stderr, "GLFW Error %d: %s\n", error, description );
}

bool emu_reset;

bool inspector_sent_reset ( void )
{
	return emu_reset;
}

void emu_done_reset ( void )
{
	emu_reset = false;
}

/* eva registers */
int eva_r0, eva_r1, eva_r2, eva_r3, eva_flags, eva_pc, eva_addr_bank, eva_addr, eva_data;
void inspector_transfer_eva_registers ( int r0, int r1, int r2, int r3, int flags, int pc, int addr_bank, int addr, int data )
{
	eva_r0 = r0;
	eva_r1 = r1;
	eva_r2 = r2;
	eva_r3 = r3;
	eva_flags = flags;
	eva_pc = pc;
	eva_addr_bank = addr_bank;
	eva_addr = addr;
	eva_data = data;
} 

static void glfw_key_callback ( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    if ( key == GLFW_KEY_TAB && action == GLFW_PRESS ) emu_reset = true;
}

ImGuiIO inspector_imgui_io;
GLFWwindow* inspector_window;
id <MTLDevice> mtl_device;
id <MTLCommandQueue> mtl_command_queue;
NSWindow *nswin;
CAMetalLayer *mtl_layer;
MTLRenderPassDescriptor *mtl_render_pass_descriptor;
int init_inspector_window ( void )
{
	/* ImGui context */
	IMGUI_CHECKVERSION ();
	ImGui::CreateContext ();
	inspector_imgui_io = ImGui::GetIO (); ( void ) inspector_imgui_io;
	inspector_imgui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	/* style */
	ImGui::StyleColorsDark ();
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Inactive title bar
    	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Active title bar
    	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Collapsed window title bar;

	/* setup GLFW window */
	glfwSetErrorCallback ( glfw_error_callback );
	if ( !glfwInit () ) return 1;
	glfwWindowHint ( GLFW_CLIENT_API, GLFW_NO_API );
	inspector_window = glfwCreateWindow ( 800, 1000, "inspector", nullptr, nullptr );
	if ( inspector_window == nullptr ) return 1;
	glfwSetKeyCallback ( inspector_window, glfw_key_callback );
	/* MTL device info */
	mtl_device = MTLCreateSystemDefaultDevice ();
	mtl_command_queue = [mtl_device newCommandQueue];
		
	/* platform renderer backends */
	ImGui_ImplGlfw_InitForOpenGL ( inspector_window, true );
	ImGui_ImplMetal_Init ( mtl_device );

	nswin = glfwGetCocoaWindow ( inspector_window );
	mtl_layer = [CAMetalLayer layer];
	mtl_layer.device = mtl_device;
	mtl_layer.pixelFormat = MTLPixelFormatRGBA8Unorm;
	nswin.contentView.layer = mtl_layer;
	nswin.contentView.wantsLayer = YES;
	
	mtl_render_pass_descriptor = [MTLRenderPassDescriptor new];	

	return 0;
}

bool eva_monitor_active = true;
void render_eva_monitor ( void )
{
	ImGui::SetNextWindowSize ( ImVec2 ( 346, 256 ), ImGuiCond_None );
	ImGui::SetNextWindowPos ( ImVec2 ( 20, 20 ) );
	ImGui::Begin ( "eva", &eva_monitor_active, ImGuiWindowFlags_NoResize );
	ImGui::SeparatorText ( "registers" );
	ImGui::Text ( "r0:%08X r1:%08X r2:%08X r3:%08X", eva_r0, eva_r1, eva_r2, eva_r3 );
	ImGui::Text ( "flags:%04X pc:%02X", eva_flags, eva_pc );
	ImGui::SeparatorText ( "ports" );
	ImGui::Text ( "bank:%02X addr:%04X data:%08X", eva_addr_bank, eva_addr, eva_data );
	ImGui::End ();
}

void imgui_loop ( void )
{
	render_eva_monitor ();
}

float clear_color [4] = { 0.149f, 0.145f, 0.137f, 1.0f };
void update_inspector_window ( void )
{
	/* call updates to GLFW and MTL */
	glfwPollEvents ();

	int width, height;
	glfwGetFramebufferSize ( inspector_window, &width, &height );
	mtl_layer.drawableSize = CGSizeMake ( width, height );
	id <CAMetalDrawable> mtl_drawable = [mtl_layer nextDrawable];

	id <MTLCommandBuffer> mtl_command_buffer = [mtl_command_queue commandBuffer];
        mtl_render_pass_descriptor.colorAttachments[0].clearColor = MTLClearColorMake(clear_color[0] * clear_color[3], 
							clear_color[1] * clear_color[3], clear_color[2] * clear_color[3], clear_color[3]);
        mtl_render_pass_descriptor.colorAttachments[0].texture = mtl_drawable.texture;
       	mtl_render_pass_descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
       	mtl_render_pass_descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        id <MTLRenderCommandEncoder> mtl_render_encoder = [mtl_command_buffer renderCommandEncoderWithDescriptor:mtl_render_pass_descriptor];
        [mtl_render_encoder pushDebugGroup:@"ImGui demo"];

        /* start ImGui frame */
        ImGui_ImplMetal_NewFrame ( mtl_render_pass_descriptor );
       	ImGui_ImplGlfw_NewFrame ();
        ImGui::NewFrame ();

	imgui_loop ();
	
	/* rendering */	
        ImGui::Render ();
        ImGui_ImplMetal_RenderDrawData ( ImGui::GetDrawData (), mtl_command_buffer, mtl_render_encoder);

        [mtl_render_encoder popDebugGroup];
        [mtl_render_encoder endEncoding];

        [mtl_command_buffer presentDrawable:mtl_drawable];
        [mtl_command_buffer commit];
}

void destroy_inspector_window ( void )
{	
    	ImGui_ImplMetal_Shutdown ();
    	ImGui_ImplGlfw_Shutdown ();
    	ImGui::DestroyContext ();

    	glfwDestroyWindow ( inspector_window );
    	glfwTerminate ();
}
