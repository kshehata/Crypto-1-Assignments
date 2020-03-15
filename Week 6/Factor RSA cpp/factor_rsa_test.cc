#include "factor_rsa.h"


#include <gmpxx.h>
#include <gtest/gtest.h>
#include <utility>

using std::pair;
using std::make_pair;
using std::make_unique;
using std::string;
using std::tuple;
using std::unique_ptr;
using ::testing::get;
using ::testing::TestWithParam;
using ::testing::Values;

namespace testing {


class ExpModNTest : 
    public TestWithParam<tuple<mpz_class, mpz_class, mpz_class, mpz_class, int>> {

 protected:
  ~ExpModNTest() override {}
  void SetUp() override {
    g_ = get<0>(GetParam());
    x_ = get<1>(GetParam());
    p_ = get<2>(GetParam());
    h_ = get<3>(GetParam());
    n_ = get<4>(GetParam());
  }
  void TearDown() override {}

  mpz_class g_;
  mpz_class x_;
  mpz_class p_;
  mpz_class h_;
  int n_;
};

TEST_P(ExpModNTest, ExpModN) {
  EXPECT_EQ(h_, exp_mod_n(g_, x_, p_));
}

INSTANTIATE_TEST_SUITE_P(ExpModNTestExamples,
                         ExpModNTest,
                         Values(
                          make_tuple("12345", "1234", "54323", "6640", 12),
                          make_tuple("555557", "133332", "777781", "661555", 20),
                          make_tuple(
  "1153678116233732248745990358966319550630377323767515160209467723675444630318",
  "653936872533",
  "1809251394333065553493296640760748560207343510400633813116524750123642650649",
  "1346717359610222066042143255464844450174724877494852427753510799474619764949",
  40),
                          make_tuple(
  "20361504138786840196959404340122724172133245264983173112152469958214205976"
  "432197956793699063774801435329909525308842444937248346129392079903054222"
  "30770085",
  "1073489355871",
  "67039039649712985497870124991029230637396829102961966888617807218608820150"
  "367734884009371490834517138450159290932430254268769414059732849732168245"
  "03042159",
  "2057587726143882535542688452040204377649691462476959342168008460263139675178"
  "2755612429894197132020967988022176407191283440133977007909937700691038464661"
  "19", 40)
                          ));


TEST(FactorRSATest, iroot) {
  mpz_class result;
  EXPECT_TRUE(iroot(7*7, 2, result));
  EXPECT_EQ(7, result);
  EXPECT_FALSE(iroot(50, 2, result));
  EXPECT_EQ(7, result);
}

class FactorRSATest : 
    public TestWithParam<pair<mpz_class, mpz_class>> {

 protected:
  ~FactorRSATest() override {}
  void SetUp() override {}
  void TearDown() override {}
};

TEST_P(FactorRSATest, ExpModN) {
  mpz_class p = GetParam().first;
  mpz_class q = GetParam().second;
  mpz_class p_result, q_result;
  EXPECT_TRUE(factor_rsa_modulus(p * q, p_result, q_result));
  EXPECT_EQ(p, p_result);
  EXPECT_EQ(q, q_result);
}

INSTANTIATE_TEST_SUITE_P(FactorRSATestExamples,
                         FactorRSATest,
                         Values(
                          make_pair("10007", "10103"),
                          make_pair("511351", "512353")
                          ));

const mpz_class EXAMPLE_1_N(
  "17976931348623159077293051907890247336179769789423065727343008115"
  "77326758055056206869853794492129829595855013875371640157101398586"
  "47833778606925583497541085196591615128057575940752635007475935288"
  "71082364994994077189561705436114947486504671101510156394068052754"
  "0071584560878577663743040086340742855278549092581");

const mpz_class EXAMPLE_2_N(
  "6484558428080716696628242653467722787263437207069762630604390703787"
  "9730861808111646271401527606141756919558732184025452065542490671989"
  "2428844841839353281972988531310511738648965962582821502504990264452"
  "1008852816733037111422964210278402893076574586452336833570778346897"
  "15838646088239640236866252211790085787877");

const mpz_class EXAMPLE_3_N(
  "72006226374735042527956443552558373833808445147399984182665305798191"
  "63556901883377904234086641876639384851752649940178970835240791356868"
  "77441155132015188279331812309091996246361896836573643119174094961348"
  "52463970788523879939683923036467667022162701835329944324119217381272"
  "9276147530748597302192751375739387929");

const mpz_class EXAMPLE_4_CIPHERTEXT(
  "2209645186741038177630656113488341801741006978789283107173183914367613560012"
  "0538004282329650473509424343946219751512256465839967942889460764542040581564"
  "7489880137348641204523252293201764879166664029975091887299716905260832220677"
  "7160001932926087000957999372407745896777369781757126722995114866295962793479"
  "1540");

TEST(FactorExamples, Question1) {
  mpz_class p_result, q_result;
  EXPECT_TRUE(scan_for_factor(EXAMPLE_1_N, 1, p_result, q_result));
  EXPECT_EQ(mpz_class(
    "13407807929942597099574024998205846127479365820592393377723561443721764030"
    "07366276889111161436232699867504054609433932083841952337598602753044156213"
    "5724301"), p_result);
  EXPECT_EQ(mpz_class(
    "13407807929942597099574024998205846127479365820592393377723561443721764030"
    "07377856098034893055775056966004923400219259082308516394002548511444947526"
    "5364281"), q_result);
}

TEST(FactorExamples, Question2) {
  mpz_class p_result, q_result;
  EXPECT_TRUE(scan_for_factor(EXAMPLE_2_N, 1<<20, p_result, q_result));
  EXPECT_EQ(mpz_class(
    "25464796146996183438008816563973942229341454268524157846328581927885777969"
    "98522283514385107324957345410738446155719317330449724481407150579056659320"
    "6419759"), p_result);
  EXPECT_EQ(mpz_class(
    "25464796146996183438008816563973942229341454268524157846328581927885777970"
    "10639805449124652697081416763256350954178473474187137985668235474771834647"
    "1375403"), q_result);
}

TEST(FactorExamples, Question3) {
  mpz_class p_result, q_result;
  EXPECT_TRUE(factor_rsa_split(EXAMPLE_3_N, 6, 4, p_result, q_result));
  EXPECT_EQ(mpz_class(
    "21909849592475533092273988531583955898982176093344929030099423584127212078"
    "12615004472110257095781266512747505146508883355599329464419095529361341165"
    "8629209"), p_result);
  EXPECT_EQ(mpz_class(
    "32864774388713299638410982797375933848473264140017393545149135376190818117"
    "18924003582581649495471182162607621036411384844001228586331102742612137005"
    "0758081"), q_result);
}

TEST(FactorExamples, SplitFactor) {
  mpz_class p = 2003;
  mpz_class q = 3001;
  mpz_class p_result, q_result;
  EXPECT_TRUE(factor_rsa_split(p*q, 6, 4, p_result, q_result));
  EXPECT_EQ(p, p_result);
  EXPECT_EQ(q, q_result);
}

TEST(FactorExamples, Question4) {
  mpz_class p, q;
  EXPECT_TRUE(scan_for_factor(EXAMPLE_1_N, 1, p, q));

  EXPECT_EQ("Factoring lets us break RSA.",
    decrypt_from_factors(EXAMPLE_4_CIPHERTEXT, 65537, p, q));
}

} // namespace testing
