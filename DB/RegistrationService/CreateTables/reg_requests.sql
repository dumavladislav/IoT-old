CREATE TABLE IF NOT EXISTS RegistrationService.reg_requests (
	id SERIAL UNIQUE,
	device_macaddress macaddr,
	createDateTime timestamp
);