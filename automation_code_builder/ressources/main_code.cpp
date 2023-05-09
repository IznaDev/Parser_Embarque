#include "arduino_context.hpp"

// Global variables
ArduinoFactory factory;
DeviceDataContext dc;
const int nb_behaviors = 1;
Behavior* behaviors[1];


void setup()
{
	// Device "presence_1"
	DeviceSettings s_presence_1;
	s_presence_1.add_config("pin", 15);
	s_presence_1.add_input("detection");
	dc.add_or_set_device(&factory, "presence_1", "HC-SR501", s_presence_1);
	// Device "lampe_1"
	DeviceSettings s_lampe_1;
	s_lampe_1.add_config("pin", 10);
	s_lampe_1.add_output("intensite");
	dc.add_or_set_device(&factory, "lampe_1", "LED", s_lampe_1);
	dc.setup();
	
	Operation_Expression* if_expr = new GreaterThan_Expression();
	if_expr->add_member(new Reference_Expression("presence_1.detection"));
	if_expr->add_member(new Constant_Expression("0"));

	Operation_Expression* then_expr = new Set_Expression();
	then_expr->add_member(new Reference_Expression("lampe_1.intensite"));
	then_expr->add_member(new Constant_Expression("255"));

	Operation_Expression* else_expr = new Set_Expression();
	else_expr->add_member(new Reference_Expression("lampe_1.intensite"));
	else_expr->add_member(new Constant_Expression("0"));


	behaviors[0] = new Behavior(if_expr, then_expr, else_expr);

}

void loop()
{
	for(int i=0; i< nb_behaviors;i++)
	{
		behaviors[i]->process(&dc);
	}
}
