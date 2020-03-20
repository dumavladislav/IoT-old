CREATE TABLE IF NOT EXISTS RegistrationService.reg_requests_decision (
	id SERIAL UNIQUE,
	reg_req_id integer NOT NULL,
	createDateTime timestamp,
	decision varchar
);