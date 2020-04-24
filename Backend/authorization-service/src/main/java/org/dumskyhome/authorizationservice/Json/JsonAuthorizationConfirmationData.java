package org.dumskyhome.authorizationservice.Json;

import javax.persistence.criteria.CriteriaBuilder;
import java.util.UUID;

public class JsonAuthorizationConfirmationData {
    Boolean authorized;
    String sessionToken;

    public JsonAuthorizationConfirmationData(Boolean authorized) {
        this.authorized = authorized;
        if (getAuthorized()) {
            sessionToken = UUID.randomUUID().toString();
        }
    }

    public Boolean getAuthorized() {
        return authorized;
    }

    public void setAuthorized(Boolean authorized) {
        this.authorized = authorized;
    }

    public String getSessionToken() {
        return sessionToken;
    }

    public void setSessionToken(String sessionToken) {
        this.sessionToken = sessionToken;
    }
}
