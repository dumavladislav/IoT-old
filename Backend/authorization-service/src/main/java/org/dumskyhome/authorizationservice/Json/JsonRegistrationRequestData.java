package org.dumskyhome.authorizationservice.Json;

public class JsonRegistrationRequestData {
    private String requestType;

    public JsonRegistrationRequestData() {

    }

    public JsonRegistrationRequestData(String requestType) {
        this.requestType = requestType;
    }

    public String getRequestType() {
        return requestType;
    }

    public void setRequestType(String requestType) {
        this.requestType = requestType;
    }
}
