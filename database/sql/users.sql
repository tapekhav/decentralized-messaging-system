--
-- PostgreSQL database dump
--

-- Dumped from database version 14.10 (Ubuntu 14.10-0ubuntu0.22.04.1)
-- Dumped by pg_dump version 14.9 (Ubuntu 14.9-0ubuntu0.22.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: UserInfo; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."UserInfo" (
    info_id integer NOT NULL,
    birth_date date NOT NULL,
    user_id integer NOT NULL,
    name character varying(255) NOT NULL,
    additional_information character varying(255) DEFAULT NULL::character varying
);


ALTER TABLE public."UserInfo" OWNER TO postgres;

--
-- Name: Users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Users" (
    nickname character varying(255) NOT NULL,
    ip_v4 character varying(15),
    user_id integer NOT NULL
);


ALTER TABLE public."Users" OWNER TO postgres;

--
-- Data for Name: UserInfo; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."UserInfo" (info_id, birth_date, user_id, name, additional_information) FROM stdin;
\.


--
-- Data for Name: Users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Users" (nickname, ip_v4, user_id) FROM stdin;
\.


--
-- Name: UserInfo UserInfo_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."UserInfo"
    ADD CONSTRAINT "UserInfo_pkey" PRIMARY KEY (info_id);


--
-- Name: Users user_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Users"
    ADD CONSTRAINT user_id PRIMARY KEY (user_id) INCLUDE (user_id);


--
-- Name: fki_user_id; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX fki_user_id ON public."UserInfo" USING btree (user_id);


--
-- Name: UserInfo user_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."UserInfo"
    ADD CONSTRAINT user_id FOREIGN KEY (user_id) REFERENCES public."Users"(user_id);


--
-- PostgreSQL database dump complete
--

