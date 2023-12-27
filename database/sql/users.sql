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
    birth_date date NOT NULL,
    user_id integer NOT NULL,
    name character varying(60) NOT NULL,
    additional_information character varying(60) DEFAULT NULL::character varying
);


ALTER TABLE public."UserInfo" OWNER TO postgres;

--
-- Name: Users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Users" (
    nickname character varying(60) NOT NULL,
    ip_v4 character varying(15),
    user_id integer NOT NULL
);


ALTER TABLE public."Users" OWNER TO postgres;

--
-- Data for Name: UserInfo; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."UserInfo" (birth_date, user_id, name, additional_information) FROM stdin;
2003-06-27	1	Test	ahah
2003-07-27	2	Test2	яzxc
\.


--
-- Data for Name: Users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Users" (nickname, ip_v4, user_id) FROM stdin;
test	127.0.0.1	1
test2	127.0.0.2	2
\.


--
-- Name: Users user_id; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Users"
    ADD CONSTRAINT user_id PRIMARY KEY (user_id) INCLUDE (user_id);


--
-- Name: UserInfo user_id_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."UserInfo"
    ADD CONSTRAINT user_id_pk PRIMARY KEY (user_id) INCLUDE (user_id);


--
-- Name: fki_user_id; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX fki_user_id ON public."UserInfo" USING btree (user_id);


--
-- Name: fki_user_id_fk; Type: INDEX; Schema: public; Owner: postgres
--

CREATE INDEX fki_user_id_fk ON public."UserInfo" USING btree (user_id);


--
-- Name: UserInfo user_id_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."UserInfo"
    ADD CONSTRAINT user_id_fk FOREIGN KEY (user_id) REFERENCES public."Users"(user_id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

